/*
 * A205184  Period 12: repeat (1, 8, 4, 9, 7, 8, 7, 9, 4, 8, 1, 9).
 * https://oeis.org/A205184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-12); period 12: (1, 8, 4, 9, 7, 8, 7, 9, 4, 8, 1, 9)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* sequence values repeat every 12 terms */
    static const seq_t period[12] = {
        (seq_t)1, (seq_t)8, (seq_t)4, (seq_t)9, (seq_t)7,
        (seq_t)8, (seq_t)7, (seq_t)9, (seq_t)4, (seq_t)8,
        (seq_t)1, (seq_t)9
    };

    if (n < (idx_t)1)        /* undefined for n < 1, return 0 */
        return (seq_t)0;

    /* (n-1) % 12 gives index 0..11 for n >= 1 */
    idx_t idx = (n - (idx_t)1) % ((idx_t)12);
    return period[idx];
}
