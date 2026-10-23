/*
 * A040394  Continued fraction for sqrt(415).
 * https://oeis.org/A040394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms for sqrt(415)
   a(0) = 20
   For n>0: a(n) = period[(n-1) % 16],
   where period = {2,1,2,4,6,1,1,3,1,1,6,4,2,1,2,40}
*/
seq_t a(idx_t n)
{
    static const seq_t period[16] = {
        (seq_t)2, (seq_t)1, (seq_t)2, (seq_t)4,
        (seq_t)6, (seq_t)1, (seq_t)1, (seq_t)3,
        (seq_t)1, (seq_t)1, (seq_t)6, (seq_t)4,
        (seq_t)2, (seq_t)1, (seq_t)2, (seq_t)40
    };
    if (n == 0) {
        return (seq_t)20;
    } else {
        idx_t idx = (n - 1) % 16;
        return period[idx];
    }
}
