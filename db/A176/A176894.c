/*
 * A176894  Increase each digit in the binary representation of n by 2.
 * https://oeis.org/A176894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176894:
   a(n) = decimal number obtained by adding 2 to each binary digit of n. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;      /* binary "0" -> "2" */

    /* find the position of the most significant '1' bit */
    unsigned msb = 0;
    idx_t tmp = n;
    while ( (tmp >>= 1) != 0 )
        ++msb;

    seq_t res = (seq_t)0;
    for (int i = (int)msb; i >= 0; --i)
    {
        seq_t digit = ((n >> i) & 1) ? (seq_t)3 : (seq_t)2;
        res = res * (seq_t)10 + digit;
    }
    return res;
}
