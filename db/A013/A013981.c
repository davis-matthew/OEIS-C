/*
 * A013981  Number of commutative elements in Coxeter group H_n.
 * https://oeis.org/A013981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(2n+2, n+1) - 2^(n+2) + n + 3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute central binomial coefficient C(2n+2, n+1) */
    seq_t binom = 1;
    for (idx_t k = 1; k <= n + 1; ++k) {
        seq_t mult = (seq_t)(n + 1 + k);          /* (n+1)+k */
        if (sizeof(seq_t) <= 8) {
            /* For 8‑byte or smaller types use a 128‑bit intermediate
               to avoid overflow before the exact division.           */
            unsigned __int128 prod = (unsigned __int128)binom * (unsigned __int128)mult;
            binom = (seq_t)(prod / (seq_t)k);
        } else {
            /* For larger types (arbitrary precision) the normal
               multiplication/division works without overflow.          */
            binom = (binom * mult) / (seq_t)k;
        }
    }

    /* Compute 2^(n+2). The shift is performed on an
       integer of sufficient width.                            */
    seq_t pow2 = ((seq_t)1) << (n + 2);

    /* Final formula.  The additions and subtraction are
       performed in seq_t, which is unsigned.                  */
    return binom - pow2 + (seq_t)n + (seq_t)3;
}
