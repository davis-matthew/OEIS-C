/*
 * A014709  The regular paper-folding (or dragon curve) sequence. Alphabet
 *          {1,2}.
 * https://oeis.org/A014709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(4n)=1, a(4n+2)=2, a(2n+1)=a(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;
    while (m & 1)          /* strip trailing 1 bits until even */
        m >>= 1;
    return ((m & 3) == 0) ? (seq_t)1 : (seq_t)2;
}
