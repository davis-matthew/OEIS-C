/*
 * A225232  The number of FO3C2 moves required to restore a packet of n
 *          playing cards to its original state (order and orientation).
 * https://oeis.org/A225232
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-1 if n is odd; a(n) = (n-2)*n/2 if n is even
 * Range:     u8 n <= 23, u16 n <= 363, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;        /* behaviour below offset is undefined */
    if (n & 1)                  /* odd n */
        return (seq_t)(n - 1);
    else                        /* even n */
    {
        seq_t nn = (seq_t)n;
        seq_t n2 = nn - 2;      /* (n-2) as seq_t */
        return (n2 * nn) / 2;
    }
}
