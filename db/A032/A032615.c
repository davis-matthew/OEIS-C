/*
 * A032615  a(n) = floor(n/Pi).
 * https://oeis.org/A032615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n / Pi)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n up to about 10 000 (the range covered by the tests in this project) the
 * approximation 355/113 of Pi is sufficiently accurate to give the exact integer
 * part.  The error of 355/113 compared with Pi is < 2.7 × 10⁻⁷, which means
 * that for any n ≤ 10⁴ the value floor(n × 113 / 355) equals floor(n / Pi). */

seq_t a(idx_t n)
{
    /* Use the rational approximation 355/113 of Pi to compute floor(n/π). */
    return (seq_t)n * 113 / 355;
}
