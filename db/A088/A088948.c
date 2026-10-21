/*
 * A088948  Numbers k such that (A006530(k) + A020639(k))/2 is an integer;
 *          that is, arithmetic mean of least and largest prime factor is an
 *          integer.
 * https://oeis.org/A088948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088948:
   Numbers k such that (least prime factor of k + greatest prime factor of k)/2
   is an integer.
   This holds iff the least and greatest prime factors have the same parity,
   i.e., k is odd (both factors odd) or k is a power of two (both factors 2).
   Therefore the sequence is the union of all odd integers (≥3) and all
   powers of two (≥2).  The function generates the n-th member of this
   sorted list.
*/
seq_t a(idx_t n)
{
    idx_t  count = 0;   /* how many members found */
    seq_t  x = 1;       /* current integer to test */

    while (count < n)
    {
        ++x;                               /* move to next integer */
        /* accept if odd or a power of two */
        if ( ( (x & 1) == 1 ) || ( (x & (x - 1)) == 0 ) )
        {
            ++count;
        }
    }
    return x;
}
