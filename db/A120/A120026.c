/*
 * A120026  Decimal expansion of the value of Minkowski's question mark
 *          function at the base of the natural logarithm.
 * https://oeis.org/A120026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A120026: decimal expansion of Minkowski’s question mark
 *  function at the base of the natural logarithm.
 *  The value is 2 + 2 Σ_{k≥0} (-1)^k / 2^{f_k},
 *  where the exponents f_k are given by the three quadratic sequences below.
 *  This program evaluates the series exactly, scales it by a large power of
 *  two, and then extracts decimal digits one by one.
 *  The computation is performed only once and the digits are cached
 *  for all subsequent calls to a(idx_t).
 */

#include <stdint.h>
#include <stddef.h>

/* add 1 << pow to |a| (pow may be > 32) */
static void bi_add_pow2(uint32_t *a, size_t *len, size_t pow) {
    size_t limb  = pow >> 5;          /* pow / 32 */
    uint32_t bit = 1u << (pow & 31);  /* pow % 32 */

    if (*len <= limb) *len = limb + 1;

    uint64_t tmp = (uint64_t)a[limb] + bit;
    a[limb] = (uint32_t)tmp;
    uint64_t carry = tmp >> 32;

    size_t i = limb + 1;
    while (carry) {
        uint64_t val = (uint64_t)a[i] + carry;
        a[i] = (uint32_t)val;
        carry = val >> 32;
        ++i;
    }
}

/* multiply big integer by a small multiplier (≤10) */
static void bi_mul_small(uint32_t *a, size_t *len, uint32_t mul) {
    uint64_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        uint64_t prod = (uint64_t)a[i] * mul + carry;
        a[i] = (uint32_t)prod;
        carry = prod >> 32;
    }
    if (carry) {
        a[*len] = (uint32_t)carry;
        ++(*len);
    }
}

/* right shift by `shift` bits and return the low 32 bits of the shifted
 * result (used to extract a digit after division by 2^shift). */
static uint32_t bi_shift_right_small(const uint32_t *a, size_t len, size_t shift) {
    size_t limb  = shift >> 5;           /* shift / 32 */
    size_t offset = shift & 31;           /* shift % 32 */
    if (limb >= len)
        return 0;
    uint64_t val = a[limb];
    if (limb + 1 < len)
        val |= (uint64_t)a[limb + 1] << 32;
    return (uint32_t)(val >> offset);
}

/* compute res = a - b  (a ≥ b)  and store result in res.
 * res may be the same buffer as a. */
static void bi_sub_abs(uint32_t *res, size_t *res_len,
                       const uint32_t *a, size_t a_len,
                       const uint32_t *b, size_t b_len) {
    *res_len = a_len;
    int64_t borrow = 0;
    for (size_t i = 0; i < a_len; ++i) {
        int64_t diff = (int64_t)a[i] - borrow -
                       (i < b_len ? b[i] : 0);
        if (diff < 0) {
            diff += (1LL << 32);
            borrow = 1;
        } else
            borrow = 0;
        res[i] = (uint32_t)diff;
    }
    while (*res_len && res[*res_len-1] == 0)
        (*res_len)--;
}

/* subtract q * 2^shift from big integer a (q < 10). */
static void bi_sub_shift_small(uint32_t *a, size_t *len,
                               uint32_t q, size_t shift) {
    size_t limb  = shift >> 5;
    size_t offset = shift & 31;
    uint64_t sub = (uint64_t)q << offset;
    if (limb >= *len)
        return;

    uint64_t diff = (uint64_t)a[limb] - sub;
    a[limb] = (uint32_t)diff;
    int64_t borrow = (diff >> 63) & 1;   /* if underflow */

    size_t i = limb + 1;
    while (borrow && i < *len) {
        uint64_t d = (uint64_t)a[i] - borrow;
        a[i] = (uint32_t)d;
        borrow = (d >> 63) & 1;
        ++i;
    }
    while (*len && a[*len-1] == 0)
        (*len)--;
}

/* initialise big integer to zero */
static void bi_zero(uint32_t *a, size_t *len) {
    *len = 0;
}

/* ---------- parameters ---------- */
#define MAX_DIGITS      12000      /* produce well over 10 000 digits */
#define EXTRA_DIGITS     200       /* safety margin for rounding */
#define DIG_SIZE        (MAX_DIGITS + EXTRA_DIGITS + 2) /* 1‑based array */
#define LIMB_CAP       2500          /* number of 32‑bit limbs               */

/* ---------- storage ---------- */
static uint8_t        dig[DIG_SIZE];           /* decimal digits (1‑based)           */
static int            initialized = 0;         /* digits computed?                   */

/* big‑integer limbs */
static uint32_t       pos_arr[LIMB_CAP];
static uint32_t       neg_arr[LIMB_CAP];
static uint32_t       sum_arr[LIMB_CAP];
static size_t         pos_len, neg_len, sum_len;

/* ---------- helpers for big integers ---------- */

/* ---------- compute all required digits once ---------- */
static void compute_digits(void) {
    /* number of digits we actually compute */
    const size_t digits_needed = MAX_DIGITS + EXTRA_DIGITS + 1;

    /* choose a bit‑length M that comfortably exceeds the required
     * number of binary bits for the fractional part.
     * Since log2(10) ≈ 3.322, 4 bits per decimal digit is a safe bound. */
    const size_t M = (digits_needed * 4) + 20;   /* extra safety margin */
    const size_t K = M;                          /* shift for division by 2^M */

    /* initialise arrays */
    bi_zero(pos_arr, &pos_len);
    bi_zero(neg_arr, &neg_len);
    bi_zero(sum_arr, &sum_len);

    /* Sequence 1: k = 3,6,9,…   (k % 3 == 0)   sign = (-1)^k */
    for (uint64_t k = 3; ; k += 3) {
        uint64_t exp = (k * k) / 9 + k - 1;
        if (exp > M) break;
        size_t shift = M - (size_t)exp;
        if (k & 1)
            bi_add_pow2(neg_arr, &neg_len, shift);   /* negative sign */
        else
            bi_add_pow2(pos_arr, &pos_len, shift);   /* positive sign */
    }

    /* Sequence 2: k = 4,7,10,…  (k % 3 == 1)   sign = (-1)^k */
    for (uint64_t k = 4; ; k += 3) {
        uint64_t exp = ((k + 8) * (k - 1)) / 9;
        if (exp > M) break;
        size_t shift = M - (size_t)exp;
        if (k & 1)
            bi_add_pow2(neg_arr, &neg_len, shift);
        else
            bi_add_pow2(pos_arr, &pos_len, shift);
    }

    /* Sequence 3: k = 2,5,8,…   (k % 3 == 2)   sign = (-1)^k */
    for (uint64_t k = 2; ; k += 3) {
        uint64_t exp = (k * k + 5 * k - 5) / 9;
        if (exp > M) break;
        size_t shift = M - (size_t)exp;
        if (k & 1)
            bi_add_pow2(neg_arr, &neg_len, shift);
        else
            bi_add_pow2(pos_arr, &pos_len, shift);
    }

    /* sum = pos_arr - neg_arr  (pos_arr ≥ neg_arr for this series)   */
    bi_sub_abs(sum_arr, &sum_len, pos_arr, pos_len, neg_arr, neg_len);

    /* multiply by 2 to get 2 * Σ (-1)^k / 2^{f_k}  – the fractional part */
    bi_mul_small(sum_arr, &sum_len, 2);

    /* integer part of the value is 2 */
    dig[1] = 2;

    /* extract decimal digits one after another */
    for (size_t i = 2; i <= digits_needed; ++i) {
        bi_mul_small(sum_arr, &sum_len, 10);                /* *10 */
        uint32_t q = bi_shift_right_small(sum_arr, sum_len, K); /* /2^M */
        dig[i] = (uint8_t)q;                                  /* digit */
        bi_sub_shift_small(sum_arr, &sum_len, q, K);          /* subtract */
    }

    initialized = 1;
}

/* ---------- required entry point ---------- */
seq_t a(idx_t n)
{
    if (!initialized) compute_digits();

    if (n == 0)          /* undefined below offset */
        return (seq_t)0;
    if (n >= DIG_SIZE)   /* outside our pre‑computed range */
        return (seq_t)0;

    return (seq_t)dig[n];
}
