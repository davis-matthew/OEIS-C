/*
 * A201488  Decimal expansion of maximal quantum success probability of the
 *          CHSH game.
 * https://oeis.org/A201488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, nice
 * Formula:   a(n) = the n-th decimal digit (0‑based) of (1 + 1/sqrt(2))/2
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Maximum number of digits needed for the sieve (enough for 10 000 decimal digits) */
#define MAX_DIGITS 20000
#define BASE 1000000000U      /* 1e9, base for big integer limbs */

/*------------------------------------------------------------
   Simple big‑integer type big enough for 10 000 decimal digits
   (≈ 1114 limbs of 1e9).  All operations are performed on
   little‑endian limbs.
  ------------------------------------------------------------*/
typedef struct {
    size_t len;                /* number of used limbs */
    uint32_t d[MAX_DIGITS / 9 + 2];
} BigInt;

/* set to zero */
static void bi_set_zero(BigInt *x) {
    x->len = 1;
    x->d[0] = 0;
}

/* set to unsigned integer value */
static void bi_set_uint(BigInt *x, uint32_t v) {
    x->len = 1;
    x->d[0] = v;
}

/* copy */
static void bi_copy(const BigInt *src, BigInt *dst) {
    dst->len = src->len;
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

/* multiply by small integer (in‑place) */
static void bi_mul_small(BigInt *x, uint32_t m) {
    if (m == 1) return;
    if (m == 0) { bi_set_zero(x); return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t prod = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)(prod % BASE);
        carry  = prod / BASE;
    }
    while (carry) {
        x->d[x->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* add small integer to big integer (in‑place) */
static void bi_add_small(BigInt *x, uint32_t add) {
    uint64_t carry = add;
    size_t i = 0;
    while (carry && i < x->len) {
        uint64_t sum = (uint64_t)x->d[i] + carry;
        x->d[i] = (uint32_t)(sum % BASE);
        carry  = sum / BASE;
        ++i;
    }
    if (carry) {
        x->d[x->len++] = (uint32_t)carry;
    }
}

/* compare: -1 if a<b, 0 if equal, 1 if a>b */
static int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i]) return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* subtract b from a (a>=b), result stored in a */
static void bi_sub_self(BigInt *a, const BigInt *b) {
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t sub = (int64_t)a->d[i] - borrow -
                      (i < b->len ? b->d[i] : 0);
        if (sub < 0) {
            sub += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->d[i] = (uint32_t)sub;
    }
    while (a->len > 1 && a->d[a->len - 1] == 0) --a->len;
}

/* multiply by small integer and add small square: res = src*m + m*m */
static void bi_mul_small_add_small(const BigInt *src, uint32_t m, BigInt *res) {
    uint64_t carry = 0;
    res->len = src->len;
    for (size_t i = 0; i < src->len; ++i) {
        uint64_t prod = (uint64_t)src->d[i] * m + carry;
        res->d[i] = (uint32_t)(prod % BASE);
        carry    = prod / BASE;
    }
    while (carry) {
        res->d[res->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
    uint32_t add = m * m;
    if (add) bi_add_small(res, add);
}

/*------------------------------------------------------------
   Sequence computation state
  ------------------------------------------------------------*/
static unsigned char *digits_cache = NULL;
static size_t digits_cap = 0;
static size_t digits_len = 0;

/* sqrt(2) extraction state (integer part and remainder) */
static BigInt sqrt_p;     /* current integer part of sqrt(2)*10^k */
static BigInt sqrt_r;     /* current remainder after bringing down two zeros */

/* temporary big ints for intermediate calculations */
static BigInt c, tmp;

/* digit recurrence state: T_{k-1} mod 4 */
static uint32_t rec_r_mod4 = 0;

/* initialization flag */
static int init_done = 0;

/* enlarge storage for digit cache */
static void ensure_digits_capacity(size_t needed) {
    if (needed <= digits_cap) return;
    size_t new_cap = digits_cap ? digits_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    unsigned char *newp = (unsigned char*)realloc(digits_cache, new_cap);
    digits_cache = newp;
    digits_cap = new_cap;
}

/* produce digits up to 'need' (need = n+1) */
static void compute_more_digits(size_t need) {
    if (!init_done) {
        /* Integer part of sqrt(2) is 1.  Compute preliminary state. */
        bi_set_uint(&sqrt_r, 2);           /* remainder before integer part */
        uint32_t int_digit = 1;            /* floor(sqrt(2)) */
        bi_set_uint(&sqrt_p, int_digit);   /* p = 1 */
        /* 2 - 1^2 = 1 */
        sqrt_r.d[0] -= int_digit * int_digit;
        sqrt_r.len = 1;
        /* For the recurrence: T0 = 3 (since (2*1+1)=3) */
        rec_r_mod4 = 3;
        init_done = 1;
    }

    if (need <= digits_len) return;
    ensure_digits_capacity(need);

    /* generate digits until we reach 'need' */
    while (digits_len < need) {
        /* bring down next two zeros: r = r * 100 */
        bi_mul_small(&sqrt_r, 100);

        /* c = p * 20 */
        bi_copy(&sqrt_p, &c);
        bi_mul_small(&c, 20);

        /* find next decimal digit of sqrt(2) */
        int chosen = 0;
        for (int d = 9; d >= 0; --d) {
            bi_mul_small_add_small(&c, (uint32_t)d, &tmp);   /* tmp = (c + d) * d */
            if (bi_cmp(&tmp, &sqrt_r) <= 0) {
                chosen = d;
                break;
            }
        }

        /* update remainder and next sqrt integer */
        bi_sub_self(&sqrt_r, &tmp);           /* r = r - tmp */
        bi_mul_small(&sqrt_p, 10);            /* p = p * 10 */
        bi_add_small(&sqrt_p, (uint32_t)chosen);  /* p = p + chosen */

        /* recurrence for sequence digit: a(n) = floor( (10 * T_{n-1} + chosen) / 4 ) */
        uint32_t next_digit = (10u * rec_r_mod4 + (uint32_t)chosen) / 4u;
        rec_r_mod4 = (10u * rec_r_mod4 + (uint32_t)chosen) % 4u;
        digits_cache[digits_len++] = (unsigned char)next_digit;
    }
}

/*------------------------------------------------------------
   OEIS function
  ------------------------------------------------------------*/
seq_t a(idx_t n) {
    compute_more_digits((size_t)n + 1);
    return (seq_t)digits_cache[(size_t)n];
}
