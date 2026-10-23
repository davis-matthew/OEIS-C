/*
 * A180880  Number of permutations p() of 1..n+3 with centered difference
 *          p(i+1)-p(i-1) < 0 exactly twice.
 * https://oeis.org/A180880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   binom, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = C(n+3, L_o) * Σ_{i=0}^2 A(L_o, i) * A(L_e, 2-i)
 * where L_o = ceil((n+3)/2), L_e = floor((n+3)/2) and
 * A(k, d) is the Eulerian number (number of permutations of k
 * elements with exactly d descents).
 *
 * The Eulerian numbers for d = 0,1,2 are:
 *   A(k,0) = 1
 *   A(k,1) = 2^k - k - 1
 *   A(k,2) = 3^k - (k+1)·2^k + binom(k+1, 2)
 *
 * All arithmetic is done with the arbitrary‑precision type seq_t.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow_uint.h"

/* Compute the Eulerian number A(k,d) for d = 0,1,2 */
static seq_t euler(idx_t k, int d)
{
    switch (d) {
        case 0:  /* exactly 0 descents */
            return (seq_t)1;
        case 1:  /* exactly 1 descent */
            return pow_uint(2, k) - (seq_t)k - 1;
        case 2:  /* exactly 2 descents */
        {
            seq_t p2 = pow_uint(2, k);
            seq_t p3 = pow_uint(3, k);
            seq_t comb = binom(k + 1, 2);             /* (k+1 choose 2) */
            return p3 + comb - ((seq_t)(k + 1) * p2); /* = 3^k - (k+1)2^k + C(k+1,2) */
        }
        default:
            return (seq_t)0;  /* not needed for this sequence */
    }
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Length of the permutation */
    idx_t m = n + 3;

    /* Lengths of the odd‑ and even‑indexed subsequences */
    idx_t L_o = (m + 1) / 2;  /* ceil(m/2) */
    idx_t L_e = m / 2;        /* floor(m/2) */

    /* Binomial coefficient C(m, L_o) */
    seq_t bin = binom(m, L_o);

    /* Eulerian numbers for the odd subsequence */
    seq_t eo0 = euler(L_o, 0);  /* 1 */
    seq_t eo1 = euler(L_o, 1);
    seq_t eo2 = euler(L_o, 2);

    /* Eulerian numbers for the even subsequence */
    seq_t ee0 = euler(L_e, 0);  /* 1 */
    seq_t ee1 = euler(L_e, 1);
    seq_t ee2 = euler(L_e, 2);

    /* Sum over d_o + d_e = 2 */
    seq_t term = eo0 * ee2 + eo1 * ee1 + eo2 * ee0;

    /* Final result */
    return bin * term;
}
