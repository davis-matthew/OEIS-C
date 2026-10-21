/*
 * A214210  Trebled Thue-Morse sequence: the A010060 sequence replacing 0 with
 *          0,0,0 and 1 with 1,1,1.
 * https://oeis.org/A214210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = parity(popcount(floor(n/3)))
 * Range:     u8 n <= 190, u16 n <= 190, u32 n <= 190, u64 n <= 190,
 *            u128 n <= 190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 3;             /* floor division of n by 3 */
    seq_t parity = 0;            /* result (0 or 1) */

    /* Compute the parity of the binary digits of k */
    while (k) {
        parity ^= (seq_t)(k & 1);
        k >>= 1;
    }

    return parity;
}
