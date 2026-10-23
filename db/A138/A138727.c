/*
 * A138727  Numbers n whose hexadecimal representation is the n-th member of
 *          the alphanumeric sequence: 1A, 2A, 3B, 4B, 5B, 6B, 7B, 8B, 8B, 8B,
 *          1B, 2B, 3A, 4A, 5A, 6A, 7A, 8A. (A notation convention of the
 *          groups in the periodic table of the elements with 18 columns).
 * https://oeis.org/A138727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   a(n) = 16 * f(n) + g(n) where f(n) = ( (n-1)%18 <= 7 ) ?
 *            ((n-1)%18 + 1) : ( (n-1)%18 <= 9 ) ? 8 : ((n-1)%18 - 9) ) and
 *            g(n) = ( (n-1)%18 <= 1 ) ? 10 : ( (n-1)%18 <= 11 ) ? 11 : 10 (10
 *            represents 'A', 11 represents 'B')
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* undefined for n < 1, but guard just in case */

    idx_t k = (n - 1) % 18;
    idx_t numeric;
    if (k <= 7)
        numeric = k + 1;
    else if (k <= 9)
        numeric = 8;
    else
        numeric = k - 9;

    idx_t letter = (k <= 1) ? 10 : (k <= 11 ? 11 : 10);

    return ((seq_t)numeric << 4) | (seq_t)letter;
}
