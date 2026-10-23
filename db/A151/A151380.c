/*
 * A151380  Number of walks within N^2 (the first quadrant of Z^2) starting at
 *          (0,0), ending on the vertical axis and consisting of n steps taken
 *          from {(-1, 1), (0, -1), (1, 1)}.
 * https://oeis.org/A151380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = Σ_{k=⌈n/4⌉}^{⌊n/2⌋} C_k · ((4k−n+1)/(2k+1)) ·
 *            binom(n,2k), where C_k is the k‑th Catalan number. The Σ runs
 *            over admissible k
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 45, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>


/* Helper:  multiply seq_t values, using 128‑bit when available. */
static inline seq_t mul_big(seq_t a, seq_t b)
{
#ifdef __SIZEOF_INT128__
    if (sizeof(seq_t) <= 8)
        return (seq_t)(((__uint128_t)a) * b);
#endif
    return a * b;
}

/* Multiplicative binomial coefficient (n choose k).  Uses only integer ops. */
static seq_t binom_idxn_val(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = mul_big(res, (seq_t)(n - k + i));
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Determine admissible k. */
    idx_t k_min = (n + 3) / 4;   /* ceil(n/4)   */
    idx_t k_max = n / 2;         /* floor(n/2)   */
    if (k_min > k_max) return 0;

    /* Catalan number C_{k_min}.  Start from C_0 = 1. */
    seq_t catalan = 1;
    for (idx_t i = 1; i <= k_min; ++i) {
        catalan = mul_big(catalan, (seq_t)(2 * (2 * i - 1)));
        catalan = catalan / (seq_t)(i + 1);
    }

    /* Binomial coefficient binom(n, 2*k_min). */
    seq_t bin_k = binom_idxn_val(n, 2 * k_min);

    seq_t result = 0;

    for (idx_t k = k_min; k <= k_max; ++k)
    {
        if (k > k_min) {
            /* Update Catalan to C_k using C_{k-1}. */
            catalan = mul_big(catalan, (seq_t)(4 * (k - 1) + 2));
            catalan = catalan / (seq_t)(k + 1);

            /* Update bin_k to binom(n, 2k) from binom(n, 2(k-1)). */
            idx_t rprev = 2 * (k - 1);          /* previous r = 2(k-1) */
            bin_k = mul_big(bin_k, (seq_t)(n - rprev));
            bin_k = mul_big(bin_k, (seq_t)(n - rprev - 1));
            bin_k = bin_k / (seq_t)(rprev + 1);
            bin_k = bin_k / (seq_t)(rprev + 2);
        }

        /* Ballot number: ((4k−n+1)/(2k+1)) · binom(n,2k) */
        seq_t numer = mul_big((seq_t)(4 * k - n + 1), bin_k);
        seq_t ballot = numer / (seq_t)(2 * k + 1);

        /* Add C_k · ballot to the result. */
        result += mul_big(catalan, ballot);
    }

    return result;
}
