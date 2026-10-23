/*
 * A040671  Continued fraction for sqrt(698).
 * https://oeis.org/A040671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(698).
   a(0) = 26;
   for n >= 1, a(n) = period[(n-1) % 9] with period =
   {2, 2, 1, 1, 1, 1, 2, 2, 52}. */
seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)26;
    }
    static const seq_t period[9] = {
        (seq_t)2, (seq_t)2, (seq_t)1, (seq_t)1,
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)2,
        (seq_t)52
    };
    idx_t idx = (n - 1u) % 9u;
    return period[idx];
}
