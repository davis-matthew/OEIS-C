/*
 * A237424  Numbers of the form (10^a + 10^b + 1)/3.
 * https://oeis.org/A237424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 65, u64 n <= 210, u128 n <= 818,
 *            big any n
 * Helpers:   q_from_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237424: numbers of the form (10^a + 10^b + 1)/3 with a≥b≥0,
   listed in ascending order by increasing a, then increasing b.
   For a given index n≥1 determine (a,b) from the cumulative counts
   c(a)=∑_{k=0}^a (k+1)= (a+1)(a+2)/2.
   Then use the identity 10^k = 3·q_k + 1, where q_k = (10^k – 1)/3.
   The recurrence q_{k+1} = 10·q_k + 3 allows computation of q_k
   without overflow for all k that keep the final result in seq_t.
   Finally a(n) = q_a + q_b + 1. */
#include <stdint.h>
#include "../utils/q_from_idx.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Find smallest a such that (a+1)(a+2)/2 ≥ n.
       Use incremental cumulative count to avoid overflow. */
    idx_t a = 0;
    idx_t cumulative = 0;       /* cumulative count for previous a */
    while (cumulative + (a + 1) < n) {
        cumulative += a + 1;
        ++a;
    }

    /* b is the offset within the block for this a. */
    idx_t b = n - cumulative - 1;

    seq_t qa = q_from_idx(a);
    seq_t qb = q_from_idx(b);

    return qa + qb + 1;
}
