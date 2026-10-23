/*
 * A140975  Period 20: repeat [1, 3, 8, 8, 9, 1, 6, 6, 7, 9, 4, 4, 5, 7, 2, 2,
 *          3, 5, 0, 0].
 * https://oeis.org/A140975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = A011377(n) mod 10, which is periodic with period 20
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic pattern of length 20.  The values are the first 20 terms of
       the sequence, as defined by the OEIS entry. */
    static const seq_t period[20] = {
        1, 3, 8, 8, 9,
        1, 6, 6, 7, 9,
        4, 4, 5, 7, 2,
        2, 3, 5, 0, 0
    };
    /* n is uint64_t, modulo operation is safe. */
    unsigned int idx = (unsigned int)(n % 20ULL);
    return period[idx];
}
