/*
 * A084567  Binomial transform of (1,-1,4,-16,64,-256,1024,...) =
 *          (3*0^n-(-4)^n)/4.
 * https://oeis.org/A084567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (3 + (-3)^n)/4
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 20, i64 n <= 41, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset directly */
    if (n == 0)
        return (seq_t)1;

    /* For small built‑in integer types (≤ 64 bits) use a 128‑bit
     * intermediate to avoid overflow while still keeping the
     * result exact when it fits in seq_t.  When seq_t is an
     * arbitrary‑precision type we fall back to its own arithmetic. */
    if (sizeof(seq_t) <= 8) {
        __int128 power = 1;           /* (−3)^0 */
        for (idx_t i = 1; i <= n; ++i)
            power *= -3;              /* accumulate (−3)^n in 128‑bit */
        __int128 result = (power + 3) / 4;
        return (seq_t)result;
    } else {
        seq_t power = (seq_t)1;       /* (−3)^0 */
        for (idx_t i = 1; i <= n; ++i)
            power = power * (seq_t)(-3);   /* use seq_t arithmetic */
        seq_t result = (power + (seq_t)3) / (seq_t)4;
        return result;
    }
}
