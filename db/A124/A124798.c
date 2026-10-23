/*
 * A124798  Sequence of digits (least significant digit first) of A124797
 *          (sums of cyclic permutations of 1...n written in base n+1).
 * https://oeis.org/A124798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   cum_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A124798:  least‑significant digit of the base‑(n+1) sum of all
    cyclic permutations of 1…n, concatenated for n = 1, 2, 3, ….
    Derived pattern:
      • For even n = 2t   : digits (lsb first) = 0, t, t, …, t   (t repeated n times)
      • For odd  n = 2t+1 : digits = t+1, (n-1 times) n, t   (t = floor(n/2))
    The sequence a(n) is the infinite concatenation of these digit blocks.
*/

#include <stdint.h>
#include "../utils/cum_digits.h"

/* find minimal k such that cum_digits(k) ≥ n  (C(0)=0). */
static idx_t find_block(idx_t n)
{
    idx_t lo = 1, hi = 1;
    while (cum_digits(hi) < n) hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (cum_digits(mid) >= n) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* locate the block (value index) that contains position n */
    idx_t k   = find_block(n);                /* block index (n in OEIS notation) */
    idx_t prev = cum_digits(k - 1);           /* digits count before this block */
    idx_t pos  = n - prev - 1;                /* 0‑based position inside block */

    if (k == 1) return (seq_t)1;              /* block 1 has single digit 1 */

    idx_t half = k >> 1;                      /* floor(k/2) = t */

    if ((k & 1) == 0) {                      /* even k = 2t */
        if (pos == 0) return (seq_t)0;
        else           return (seq_t)half;
    } else {                                  /* odd k = 2t + 1 */
        if (pos == 0)          return (seq_t)(half + 1); /* t+1 */
        else if (pos == k)     return (seq_t)half;       /* t   */
        else                   return (seq_t)k;          /* middle digit = k itself */
    }
}
