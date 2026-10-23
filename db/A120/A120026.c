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
 * Helpers:   bi_add_pow2, bi_mul_small, bi_shift_right_small, bi_sub_abs,
 *            bi_sub_shift_small, bi_zero
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
#include "../utils/bi_add_pow2.h"
#include "../utils/bi_mul_small.h"
#include "../utils/bi_shift_right_small.h"
#include "../utils/bi_sub_abs.h"
#include "../utils/bi_sub_shift_small.h"
#include "../utils/bi_zero.h"

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
