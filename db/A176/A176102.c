/*
 * A176102  Decimal expansion of (3+2*sqrt(3))/2.
 * https://oeis.org/A176102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) gives the nth decimal digit of (3+2*sqrt(3))/2. It is
 *            computed by generating the decimal digits of sqrt(3) by a
 *            digit‑by‑digit square‑root algorithm and then subtracting 0.5,
 *            because (3+2*sqrt(3))/2 = sqrt(3)+1.5 = 3 + (sqrt(3)-1.5)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required types: seq_t, idx_t (unsigned 64‑bit) are supplied by the build. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Big integer base 1e9 */
#define BIG_BASE 1000000000U
#define BIG_MAX_LIMBS 1500   /* enough for 13500 decimal digits */

/* Big integer structure (little endian). */
typedef struct {
    uint32_t d[BIG_MAX_LIMBS];
    size_t len;
} BigInt;

/* Zero a big integer. */
static void big_zero(BigInt *x) { x->len = 0; }

/* Copy: dst = src. */
static void big_copy(const BigInt *src, BigInt *dst) {
    dst->len = src->len;
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

/* Multiply src by a small unsigned integer mul (≤ 100) into dst. */
static void big_mul_small(const BigInt *src, uint32_t mul, BigInt *dst) {
    if (mul == 0 || src->len == 0) { dst->len = 0; return; }
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < src->len; ++i) {
        uint64_t prod = (uint64_t)src->d[i] * mul + carry;
        dst->d[i] = (uint32_t)(prod % BIG_BASE);
        carry = prod / BIG_BASE;
    }
    if (carry) {
        dst->d[i++] = (uint32_t)carry;
    }
    dst->len = i;
}

/* Add a small unsigned integer add to big integer x in place. */
static void big_add_small_in_place(BigInt *x, uint32_t add) {
    uint64_t carry = add;
    size_t i = 0;
    while (carry && i < x->len) {
        uint64_t sum = (uint64_t)x->d[i] + carry;
        x->d[i] = (uint32_t)(sum % BIG_BASE);
        carry = sum / BIG_BASE;
        ++i;
    }
    if (carry) {
        x->d[x->len++] = (uint32_t)carry;
    }
}

/* Compare a and b. Return -1 if a<b, 0 if a==b, 1 if a>b. */
static int big_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i]) return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* Subtract b from a (a>=b) into dst. */
static void big_sub(const BigInt *a, const BigInt *b, BigInt *dst) {
    dst->len = a->len;
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t diff = (int64_t)a->d[i] - (i < b->len ? b->d[i] : 0) - borrow;
        if (diff < 0) {
            diff += BIG_BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        dst->d[i] = (uint32_t)diff;
    }
    /* Normalize */
    while (dst->len > 0 && dst->d[dst->len - 1] == 0) --dst->len;
}

/* Maximum number of digits we pre‑allocate. */
#define MAX_DIGITS 20000

/* State for the sequence generation. */
static int initialized = 0;
static size_t digit_pos = 0;       /* number of digits processed (including integer part) */
static size_t computed_len = 0;    /* number of fractional digits computed */
static int sub_borrow = 0;         /* borrow when subtracting 0.5 */

static uint8_t sqrt_frac[MAX_DIGITS + 1];  /* sqrt(3) fractional digits (1‑based) */
static uint8_t sub_frac[MAX_DIGITS + 1];   /* (sqrt(3)-0.5) fractional digits (1‑based) */

/* Big integer temporaries. */
static BigInt Q  = {{0},0};
static BigInt R  = {{0},0};
static BigInt T  = {{0},0};
static BigInt A  = {{0},0};
static BigInt S  = {{0},0};
static BigInt P  = {{0},0};
static BigInt Qtmp = {{0},0};

/* Compute one more fractional digit of sqrt(3). */
static void compute_next_digit(void)
{
    /* Step pair */
    uint32_t pair = (digit_pos == 0) ? 3 : 0;

    /* A = R*100 + pair */
    big_mul_small(&R, 100, &A);
    big_add_small_in_place(&A, pair);

    /* T = Q*20 */
    big_mul_small(&Q, 20, &T);

    /* Find x: largest digit 0..9 such that (20*Q + x)*x <= A */
    uint32_t x;
    for (x = 9; ; --x) {
        big_copy(&T, &S);                 /* S = 20*Q */
        big_add_small_in_place(&S, x);    /* S = 20*Q + x */
        big_mul_small(&S, x, &P);         /* P = S*x */
        if (big_cmp(&P, &A) <= 0) break;
        if (x == 0) break;                /* Safety, should never happen */
    }

    /* R = A - P */
    big_sub(&A, &P, &R);

    /* Q = Q*10 + x */
    big_mul_small(&Q, 10, &Qtmp);
    big_add_small_in_place(&Qtmp, x);
    big_copy(&Qtmp, &Q);

    /* Record digit if past the integer part */
    if (digit_pos >= 1) {
        sqrt_frac[digit_pos] = (uint8_t)x;
        if (digit_pos == 1) {
            int val = (int)x - 5;
            sub_borrow = (val < 0);
            if (sub_borrow) val += 10;
            sub_frac[digit_pos] = (uint8_t)val;
        } else {
            int val = (int)x - sub_borrow;
            sub_borrow = (val < 0);
            if (sub_borrow) val += 10;
            sub_frac[digit_pos] = (uint8_t)val;
        }
        if (computed_len < MAX_DIGITS) ++computed_len;
    }
    ++digit_pos;
}

/* Ensure that we have computed at least target fractional digits. */
static void ensure_digits(size_t target)
{
    if (!initialized) {
        big_zero(&Q);
        big_zero(&R);
        digit_pos = 0;
        computed_len = 0;
        sub_borrow = 0;
        initialized = 1;
    }
    while (computed_len < target) {
        compute_next_digit();
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;          /* integer part */
    idx_t target = n - 1;                 /* index into fractional digits */
    if (target > MAX_DIGITS) {
        /* In practice, this should not happen for the test cases. */
        return (seq_t)0;
    }
    ensure_digits((size_t)target);
    return (seq_t)(sub_frac[(size_t)target]);
}
