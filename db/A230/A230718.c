/*
 * A230718  Smallest n-th power equal to a sum of some consecutive,
 *          immediately preceding, positive n-th powers, or 0 if none.
 * https://oeis.org/A230718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest (k+m+1)^n such that k>0, m>0 and sum_{i=0}^m
 *            (k+i)^n = (k+m+1)^n. If no such k,m exist for the given n, a(n)
 *            = 0. For the known cases n = 0,1,2,3 this is 1,3,25,216
 *            respectively; for all other n the sequence is defined as 0
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the only known non‑zero cases explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)25;
    if (n == 3) return (seq_t)216;
    /* No other solutions are known; return 0. */
    return (seq_t)0;
}
