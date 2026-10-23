/*
 * A033105  Number of 6's when n is written in base b for 2<=b<=n+1.
 * https://oeis.org/A033105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of digit 6 in the representations of n in every
   base b with 2 <= b <= n+1.  The function loops over all admissible
   bases and for each performs a standard base conversion, incrementing
   a counter whenever a digit equal to 6 is seen. */
seq_t a(idx_t n)
{
    idx_t  total = 0;                 /* accumulator may be larger than seq_t       */
    for (idx_t b = 2; b <= n + 1; ++b) {
        idx_t tmp = n;
        while (tmp) {
            if ((tmp % b) == 6u)
                ++total;
            tmp /= b;
        }
    }
    return (seq_t)total;
}
