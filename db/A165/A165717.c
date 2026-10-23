/*
 * A165717  Integers of the form k*(5+k)/4.
 * https://oeis.org/A165717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A165717 – integers of the form k*(k+5)/4.
   a(n) = (n % 2 == 1) ? (4m + 3)*(m + 2) : (m + 1)*(4m + 9),
   where m = floor((n-1)/2).  */
seq_t a(idx_t n)
{
    if (n == 0)            /* below the offset – undefined */
        return 0;
    idx_t m_idx = (n - 1) / 2;
    seq_t m = (seq_t)m_idx;
    if (n & 1U) {  /* odd n → k = 4m + 3 */
        return ((seq_t)4 * m + 3) * (m + 2);
    } else {       /* even n → k = 4m + 4 = 4(m+1) */
        return (m + 1) * ((seq_t)4 * m + 9);
    }
}
