/*
 * A176454  Decimal expansion of (12+2*sqrt(42))/3.
 * https://oeis.org/A176454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the nth decimal digit of (12+2*sqrt(42))/3.
 * The digit is obtained from the decimal expansion of sqrt(42) by
 * iterating the integer‑only “long division” method for square roots.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BIGINT_BASE 1000000000U
#define BIGINT_MAX_LIMBS 1200      /* enough for 10 000 decimal digits */

/* Big integer in base 1e9, little‑endian limbs */
typedef struct {
    size_t len;
    uint32_t limb[BIGINT_MAX_LIMBS];
} BigInt;

/* Initialise a big integer with a small value */
static void big_init(BigInt *b, uint32_t v)
{
    b->len   = 1;
    b->limb[0] = v;
}

/* Copy a big integer */
static void big_copy(BigInt *dst, const BigInt *src)
{
    dst->len = src->len;
    memcpy(dst->limb, src->limb, src->len * sizeof(uint32_t));
}

/* Multiplication by a small integer */
static void big_mul_small(BigInt *b, uint32_t m)
{
    if (m == 0) {
        b->len   = 1;
        b->limb[0] = 0;
        return;
    }
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->limb[i] * m + carry;
        b->limb[i]  = (uint32_t)(prod % BIGINT_BASE);
        carry       = prod / BIGINT_BASE;
    }
    while (carry) {
        b->limb[b->len++] = (uint32_t)(carry % BIGINT_BASE);
        carry /= BIGINT_BASE;
    }
}

/* Addition of a small integer */
static void big_add_small(BigInt *b, uint32_t v)
{
    uint64_t carry = v;
    size_t i = 0;
    for (; i < b->len && carry; ++i) {
        uint64_t sum = (uint64_t)b->limb[i] + carry;
        b->limb[i]   = (uint32_t)(sum % BIGINT_BASE);
        carry        = sum / BIGINT_BASE;
    }
    if (carry) {
        b->limb[b->len++] = (uint32_t)carry;
    }
}

/* Subtraction (a -= b), assumes a >= b */
static void big_sub(BigInt *a, const BigInt *b)
{
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < a->len; ++i) {
        int64_t sub = (int64_t)(i < b->len ? b->limb[i] : 0);
        int64_t val = (int64_t)a->limb[i] - sub - borrow;
        if (val < 0) {
            val += BIGINT_BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->limb[i] = (uint32_t)val;
    }
    while (a->len > 1 && a->limb[a->len - 1] == 0) {
        --a->len;
    }
}

/* Comparison: returns -1 if a<b, 0 if a==b, 1 if a>b */
static int big_cmp(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len) {
        return (a->len < b->len) ? -1 : 1;
    }
    for (int i = (int)a->len - 1; i >= 0; --i) {
        if (a->limb[i] != b->limb[i]) {
            return (a->limb[i] < b->limb[i]) ? -1 : 1;
        }
    }
    return 0;
}

/* Cache of already computed digits of the sequence */
static seq_t *digit_cache = NULL;
static size_t cache_cap  = 0;   /* allocated capacity in elements */
static idx_t cache_n      = 0;   /* number of digits cached (>=0) */

/* Current state of the square‑root extraction for sqrt(42) */
static BigInt R, r;          /* R = floor(sqrt(42)*10^(k-1)), r = 42*10^(2*(k-1)) - R^2 */
static uint32_t R_mod30;     /* R mod 30 */

/* Produce the next decimal digit of sqrt(42).  Updates R, r and R_mod30.  */
static void next_sqrt_digit(int *d_out)
{
    /* Step 1: r *= 100 */
    big_mul_small(&r, 100);

    int d;
    for (d = 9; d >= 0; --d) {
        /* prod = ((20*R)+d)*d = 20*R*d + d*d */
        BigInt prod;
        big_copy(&prod, &R);
        big_mul_small(&prod, 20);
        big_mul_small(&prod, (uint32_t)d);
        uint32_t dd = (uint32_t)(d * d);
        if (dd) big_add_small(&prod, dd);

        if (big_cmp(&r, &prod) >= 0) {
            /* r -= prod */
            big_sub(&r, &prod);
            /* Update R = R*10 + d */
            uint32_t old_mod = R_mod30;
            big_mul_small(&R, 10);
            big_add_small(&R, (uint32_t)d);
            R_mod30 = ( (old_mod * 10) % 30 + d ) % 30;
            *d_out = d;
            return;
        }
    }
    /* Should never occur */
    *d_out = 0;
}

/* Ensure that digits up to index n are computed and cached */
static void ensure_cache(idx_t n)
{
    if (cache_n == 0) {             /* first initialisation */
        /* allocate cache */
        cache_cap = 16;
        digit_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!digit_cache) abort();

        /* initialise sqrt extraction */
        big_init(&R, 6);
        big_init(&r, 6);
        R_mod30 = 6 % 30;

        /* a(1) = 8 */
        cache_n = 1;
        digit_cache[0] = (seq_t)8;
    }

    while (cache_n < n) {
        /* compute next decimal digit of sqrt(42) */
        int d;
        next_sqrt_digit(&d);

        /* delta = 1 if r >= R, else 0 */
        int delta = (big_cmp(&r, &R) >= 0) ? 1 : 0;

        /* A = (2*R + delta) mod 30  */
        uint32_t A_mod30 = ((2 * R_mod30) % 30 + delta) % 30;

        /* digit for a(n) : floor(A/3)  (for n>=2) */
        seq_t digit = (seq_t)(A_mod30 / 3);

        /* Grow cache if necessary */
        if ((size_t)cache_n >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(digit_cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            digit_cache = tmp;
            cache_cap = new_cap;
        }

        digit_cache[cache_n] = digit;
        ++cache_n;
    }
}

/* Main function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but return 0 */
    ensure_cache(n);
    return digit_cache[n - 1];    /* zero‑based index */
}
