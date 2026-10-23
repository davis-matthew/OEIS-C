/*
 * A040356  Continued fraction for sqrt(376).
 * https://oeis.org/A040356
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

/* Sequence A040356: continued fraction for sqrt(376).  
   a(0) = 19; for n >= 1, a(n) = period[(n-1) mod 16] where  
   period = {2, 1, 1, 3, 1, 2, 2, 4, 2, 2, 1, 3, 1, 1, 2, 38}.  */
seq_t a(idx_t n)
{
    static const seq_t period[16] = {2, 1, 1, 3, 1, 2, 2, 4,
                                     2, 2, 1, 3, 1, 1, 2, 38};
    if (n == 0) {
        return (seq_t)19;
    }
    idx_t idx = (n - 1) % (idx_t)16;
    return period[idx];
}
