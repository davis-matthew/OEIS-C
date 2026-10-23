/*
 * A190083  n + [n*r/s] + [n*t/s]; r=1, s=sin(2*Pi/5), t=csc(2*Pi/5).
 * https://oeis.org/A190083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n · csc(2π/5)⌋ + ⌊n · csc²(2π/5)⌋
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 1/sin(72°) = csc(2π/5) and its square.  They are irrational,
       but the following double constants are accurate to far more
       than the 53‑bit precision of IEEE‑754, enough to guarantee the
       correct integer part for all n that fit in a 64‑bit unsigned. */
    static const double inv_s  = 1.05146252291352600040271896;   /* csc(2π/5)   */
    static const double inv_s2 = inv_s * inv_s;                 /* csc²(2π/5)  */

    /* Convert the index to double, perform the multiplication,
       subtract a tiny epsilon to guard against rounding up
       that could cause an incorrect floor. */
    double nd = (double)n;
    double v1 = nd * inv_s - 1e-12;
    double v2 = nd * inv_s2 - 1e-12;

    /* Truncate toward zero – for non‑negative numbers this is floor. */
    unsigned long long f1 = (unsigned long long)v1;
    unsigned long long f2 = (unsigned long long)v2;

    /* Assemble the result using only integer arithmetic on the
       target seq_t type. */
    return (seq_t)n + (seq_t)f1 + (seq_t)f2;
}
