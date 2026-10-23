/*
 * A100011  a(n+1) = a(n)*((261*a(n)^3)-(504*a(n)^2)+(283*a(n))-38)/2 for n >
 *          0, a(0) = 2.
 * https://oeis.org/A100011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 2; a(n+1) =
 *            a(n)*((261*a(n)^3)-(504*a(n)^2)+(283*a(n))-38)/2 for n > 0
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 2;                /* a(0) */
    if (n == 0)
        return val;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t t   = val;          /* t = a(n)          */
        seq_t t2  = t * t;        /* t^2               */
        seq_t t3  = t2 * t;       /* t^3               */
        seq_t fact = 261 * t3 - 504 * t2 + 283 * t - 38; /* factor   */
        val = val * fact / 2;     /* a(n+1)            */
    }
    return val;
}
