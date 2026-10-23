/*
 * A101670  Fixed point of morphism 0 -> 01, 1 -> 20, 2 -> 02.
 * https://oeis.org/A101670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; for n>=1,
   let p = a(n>>1).  If n is even, a(n) = first[p],
   where first[0]=0, first[1]=2, first[2]=0.
   If n is odd,  a(n) = second[p],
   where second[0]=1, second[1]=0, second[2]=2. */
seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)0;

    seq_t p = a(n >> 1);
    if ((n & 1ULL) == 0ULL) {           /* even index */
        if (p == (seq_t)0)
            return (seq_t)0;
        else if (p == (seq_t)1)
            return (seq_t)2;
        else /* p == 2 */
            return (seq_t)0;
    } else {                            /* odd index */
        if (p == (seq_t)0)
            return (seq_t)1;
        else if (p == (seq_t)1)
            return (seq_t)0;
        else /* p == 2 */
            return (seq_t)2;
    }
}
