/*
 * A158001  Period 6: repeat [9, 10, 9, 4, 4, 1].
 * https://oeis.org/A158001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [9, 10, 9, 4, 4, 1] repeated with period 6
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic pattern of the sequence */
    static const seq_t pattern[6] = {9, 10, 9, 4, 4, 1};

    /* Index is 1-based; compute (n-1) mod 6 to select the value. */
    idx_t idx = (n - 1) % 6;
    return pattern[idx];
}
