/*
 * A040194  Continued fraction for sqrt(209).
 * https://oeis.org/A040194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic continued fraction for sqrt(209);
   a(0) = 14;
   for n >= 1, a(n) = period[(n-1) % 8] with period = {2,5,3,2,3,5,2,28} */
seq_t a(idx_t n)
{
    static const seq_t period[8] = {2, 5, 3, 2, 3, 5, 2, 28};
    if (n == 0)
        return 14;
    /* (n-1) can be computed safely on idx_t; cast to seq_t if needed */
    idx_t idx = (n - 1) % 8;  /* 0 <= idx < 8 */
    return period[idx];
}
