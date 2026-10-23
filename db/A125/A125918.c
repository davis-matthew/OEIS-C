/*
 * A125918  Sprague-Grundy values for octal game .154.
 * https://oeis.org/A125918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 1,2; a(3) = 0; for n >= 4, * a(n) =
 *            period[(n-4) % 11] where period = {1,1,2,2,2,2,2,4,1,1,1}
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1 || n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)0;
    /* period for n >= 4: 1,1,2,2,2,2,2,4,1,1,1 */
    static const seq_t period[11] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)2,
        (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)4,
        (seq_t)1, (seq_t)1, (seq_t)1
    };
    /* compute index modulo 11 */
    idx_t idx = (n - 4) % 11;
    return period[idx];
}
