/*
 * A040489  Continued fraction for sqrt(512).
 * https://oeis.org/A040489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   Continued fraction for sqrt(512) a(0) = 22 For n >= 1, a(n) =
 *            period[(n-1) % 12] where period = {1, 1, 1, 2, 6, 11, 6, 2, 1,
 *            1, 1, 44} (The period ends with 2*a(0) = 44)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[12] = {
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)2,
        (seq_t)6, (seq_t)11, (seq_t)6, (seq_t)2,
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)44
    };
    if (n == 0)
        return (seq_t)22;
    idx_t idx = (n - 1) % 12;
    return period[idx];
}
