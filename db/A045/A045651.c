/*
 * A045651  L1-coordination sequence for E_8 lattice in Construction A version
 *          of the lattice.
 * https://oeis.org/A045651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..min(8,n)} p[k] * C(n−k+7,7) with p =
 *            {1,8,28,56,198,56,28,8,1}
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 34, u64 n <= 816, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute binomial coefficient C(a,b) for a,b ≤ 8 (or up to 7 in this formula).
   a,b are idx_t, returns seq_t. */
static seq_t binom_u64(idx_t a, idx_t b)
{
    if (a < b) return 0;
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        res = res * (seq_t)(a - b + i) / (seq_t)i;
    }
    return res;
}

/* OEIS sequence A045651: L1-coordination sequence for E_8 lattice.
   n is the index (uint64_t). */
seq_t a(idx_t n)
{
    /* coefficients of the numerator polynomial */
    static const seq_t p[9] = {
        (seq_t)1, (seq_t)8, (seq_t)28, (seq_t)56,
        (seq_t)198, (seq_t)56, (seq_t)28, (seq_t)8, (seq_t)1
    };

    idx_t lim = (n < 8 ? n : 8);  /* k runs from 0 to min(8,n) */
    seq_t sum = 0;

    for (idx_t k = 0; k <= lim; ++k) {
        idx_t a_nk = n - k + 7;          /* the 'a' argument for binomial */
        seq_t bin = binom_u64(a_nk, 7);   /* C(n-k+7, 7) */
        sum += p[k] * bin;
    }
    return sum;
}
