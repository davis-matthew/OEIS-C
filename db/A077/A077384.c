/*
 * A077384  a(1) = 1, then add, multiply, subtract, multiply 2, 3, 4, 5; 6, 7,
 *          8, 9; ... in that order.
 * https://oeis.org/A077384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1;
   a(2n+1) = (2n+1) * a(2n);
   a(4n)   = a(4n-1) - 4n;
   a(4n+2) = a(4n+1) + 4n+2;
*/

seq_t a(idx_t n)
{
    /* Fast return for the first term. */
    if (n <= 1u)
        return (seq_t)1;

    /* a(1) is the starting value.  We compute successive terms
       up to n in a single loop, never allocating extra memory. */
    seq_t cur = (seq_t)1;          /* a(1) */
    idx_t i;

    for (i = 2u; i <= n; ++i) {
        if (i & 1u) {                 /* i is odd  -> a(2n+1) = i*a(2n) */
            cur = (seq_t)i * cur;
        } else if ((i & 3u) == 0u) {  /* i % 4 == 0 -> a(4n) = a(4n-1)-i */
            cur = cur - (seq_t)i;
        } else {                      /* i % 4 == 2 -> a(4n+2) = a(4n+1)+i */
            cur = cur + (seq_t)i;
        }
    }
    return cur;
}
