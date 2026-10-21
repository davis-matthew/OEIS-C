/*
 * A221002  The Wiener index of the nanostar dendrimer G[n], defined
 *          pictorially in the Karbasioun-Ashrafi reference.
 * https://oeis.org/A221002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 0, u32 n <= 7, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A221002
   Formula: a(n) = 502 + 5*2^(n+6)*(14*n+53) + (5*n+1)*4^(n+6)
   (OEIS offset: 0) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/pow4.h"

/* Compute a(n) for n >= 0 */
seq_t a(idx_t n) {
    /* 502 fits in seq_t via cast */
    seq_t res = (seq_t)502;

    /* Compute 2^(n+6) */
    seq_t pow2_n = pow2(n + 6);

    /* term1 = 5 * pow2_n * (14*n + 53) */
    seq_t term1 = pow2_n * (seq_t)5;
    term1 = term1 * ((seq_t)(14 * n + 53));

    /* Compute 4^(n+6) as pow4(n+6) */
    seq_t pow4_n = pow4(n + 6);

    /* term2 = (5*n + 1) * pow4_n */
    seq_t term2 = pow4_n * ((seq_t)(5 * n + 1));

    /* Sum all parts */
    res += term1;
    res += term2;

    return res;
}
