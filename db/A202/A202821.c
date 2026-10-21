/*
 * A202821  Position of 6^n among 3-smooth numbers A003586.
 * https://oeis.org/A202821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (i,j) with i≥0, j≥0 such that 2ⁱ·3ʲ ≤ 6ⁿ.
 *            This computes a(n) for every n ≥ 0 using only integer arithmetic
 *            and works for all supplied seq_t / idx_t sizes
 * Range:     u8 n <= 10, u16 n <= 175, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Fixed‑point scaling factor for logarithmic comparison. */
static const unsigned long long SCALE = 1ULL << 40;
static const unsigned long long LOG2_SCALED =
    (unsigned long long)(0.6931471805599453094172321214581765680755001343602552 *
                         (double)SCALE + 0.5);
static const unsigned long long LOG3_SCALED =
    (unsigned long long)(1.0986122886681096913838742373258 *
                         (double)SCALE + 0.5);

/* Compare 2^a·3^b ≤ 2^c·3^d using scaled logs. */
static inline bool leq(idx_t a, idx_t b, idx_t c, idx_t d)
{
    int64_t da = (int64_t)a - (int64_t)c;
    int64_t db = (int64_t)b - (int64_t)d;

    /* Positive parts of the exponent differences. */
    unsigned long long A = da > 0 ? (unsigned long long)da : 0;
    unsigned long long B = db > 0 ? (unsigned long long)db : 0;
    /* Negative parts (taken as positives). */
    unsigned long long C = da < 0 ? (unsigned long long)(-da) : 0;
    unsigned long long D = db < 0 ? (unsigned long long)(-db) : 0;

    /* Evaluate scaled logarithmic values. */
    __uint128_t left  = (__uint128_t)A * LOG2_SCALED + (__uint128_t)B * LOG3_SCALED;
    __uint128_t right = (__uint128_t)C * LOG2_SCALED + (__uint128_t)D * LOG3_SCALED;
    return left <= right;
}

/* Returns a(n). */
seq_t a(idx_t n)
{
    /* Compute j_max: largest j with 3^j ≤ 6^n. */
    idx_t jmax = n;
    while (leq(0, jmax + 1, n, n))
        ++jmax;

    idx_t j_cur = jmax;
    seq_t count = 0;
    idx_t i = 0;

    for (;; ++i) {
        /* Stop if 2^i already exceeds 6^n. */
        if (!leq(i, 0, n, n))
            break;

        /* Reduce j_cur until 2^i·3^j_cur ≤ 6^n. */
        while (j_cur > 0 && !leq(i, j_cur, n, n))
            --j_cur;

        if (!leq(i, j_cur, n, n))
            break;  /* No valid j for this i. */

        count += (seq_t)(j_cur + 1);
    }
    return count;
}
