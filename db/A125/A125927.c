/*
 * A125927  Sprague-Grundy values for octal game .53.
 * https://oeis.org/A125927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0..10) are fixed: 0,1,1,2,2,1,0,2,2,4,0. For n >= 11 the
 *            sequence repeats every 9 terms: pattern = {1,2,2,1,1,2,2,4,1}.
 *            Thus a(n) = pattern[(n-11) mod 9]
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values for n = 0..10 */
    static const seq_t init[11] = {
        0, 1, 1, 2, 2, 1, 0, 2, 2, 4, 0
    };
    /* Periodic pattern for n >= 11 (period 9) */
    static const seq_t period[9] = {
        1, 2, 2, 1, 1, 2, 2, 4, 1
    };

    if (n < 11) {
        return init[n];
    }
    /* n >= 11 */
    idx_t pos = (n - 11) % 9;
    return period[pos];
}
