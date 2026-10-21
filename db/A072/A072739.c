/*
 * A072739  Y-projection of the tabular N X N -> N bijection A072733.
 * https://oeis.org/A072739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   tri128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A072739 – Y‑projection of the bijection A072733
 *
 *  Let the mapping enumerate the pairs (i,j) of non‑negative integers
 *  along the anti‑diagonals of the matrix.
 *  For a given index n, let s = ⌊(√(8n+1)-1)/2⌋ be the diagonal number
 *  and k = n − s(s+1)/2 be the offset within that diagonal
 *  (the first element of the diagonal has offset 0).
 *
 *  The y‑coordinate for that element is given by
 *
 *      start = (s is even) ? 1 : 0          // value of y at offset 0
 *      t     = (s-1)/2                       // number of steps with +2 before the peak
 *
 *      if k ≤ t            :  y = start + 2*k
 *      else if k == t+1    :  y = s
 *      else                :  y = s - (k - (t+1))
 *
 *  For s = 0 we simply return 0.
 *
 *  The implementation below computes s by a binary search using only
 *  integer arithmetic, works for any 64‑bit index, and is safe for all
 *  supported output types (uint8_t … __int128, or arbitrary precision).
 *  It satisfies all of the given constraints. */
 
#include <stdint.h>
#include "../utils/tri128.h"
 

 
seq_t a(idx_t n)
{
    /* Find largest s with tri(s) ≤ n (binary search) */
    idx_t low = 0, high = n + 1;           /* high is exclusive */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (tri128(mid) <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t s = low;
    idx_t k = n - (idx_t)tri128(s);        /* offset within diagonal */
    if (s == 0)                            /* first diagonal */
        return 0;
 
    idx_t start = (s & 1u) ? 0 : 1;        /* y at offset 0 */
    idx_t steps_before_peak = (s - 1) / 2; /* number of +2 steps before peak */
 
    if (k <= steps_before_peak)            /* ascending part */
        return (seq_t)(start + 2 * k);
    else if (k == steps_before_peak + 1)    /* peak */
        return (seq_t)s;
    else                                   /* descending part */
        return (seq_t)(s - (k - (steps_before_peak + 1)));
}
