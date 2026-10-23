/*
 * A135814  Triangle of numbers of coincidence-free length n-m lists of
 *          m-tuples with all numbers 1,...,n-m in tuple position k, for
 *          k=1..m.
 * https://oeis.org/A135814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 37, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   flat_to_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A135814
   a(n) = sum_{j=0}^{n-m} (-1)^{(n-m)-j} * binomial(n-m, j) * (j!)^m
   where the flattened index n is interpreted as the row‑major order
   of the triangular array a(n,m) with row n containing entries
   a(n,0), a(n,1), …, a(n,n).  The function computes a(n) for any
   n ≥ 0. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/flat_to_coords.h"

/* Helper: repeated‑squaring exponentiation for seq_t */
static seq_t pow_seq_local(seq_t base, int exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) result = result * base;
        exp >>= 1;
        if (exp) base = base * base;
    }
    return result;
}

seq_t a(idx_t k)
{
    /* Find the (n, m) corresponding to the flattened index k */
    idx_t n, m;
    flat_to_coords(k, &n, &m);

    /* If the index is malformed (should not happen) return 0 */
    if (m > n) return (seq_t)0;

    /* Compute a(n, m) via the inclusion‑exclusion formula. */
    idx_t diff = n - m;                /* n - m = length of the list */

    seq_t sum_pos = (seq_t)0;
    seq_t sum_neg = (seq_t)0;

    seq_t binom = (seq_t)1;            /* binomial(diff, 0) = 1 */
    seq_t fact  = (seq_t)1;            /* fact = 0! = 1  */

    for (idx_t j = 0; j <= diff; ++j) {
        /* term = binom(diff, j) * (j!)^m */
        seq_t pow_val = pow_seq_local(fact, (int)m);
        seq_t term    = binom * pow_val;

        /* Add term with alternating sign */
        if ( ((diff - j) & 1) ) {
            sum_neg += term;
        } else {
            sum_pos += term;
        }

        /* Update binom and fact for next j if needed */
        if (j < diff) {
            /* binom = binom * (diff - j) / (j + 1) */
            binom = binom * (diff - j) / (j + 1);
            fact = fact * (j + 1);       /* fact becomes (j+1)! for next j */
        }
    }

    return sum_pos - sum_neg;           /* final value is non‑negative */
}
