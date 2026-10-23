/*
 * A147998  [r]*[2r]*[3r]*...[nr], where r=(1+sqrt(5))/2 and []=floor.
 * https://oeis.org/A147998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=1}^{n} floor(r*k), where r = (1+sqrt(5))/2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for n >= 1.  Use only integer arithmetic for seq_t
 * values.  The floor of r*k is evaluated with a double temporary
 * because k <= n is limited to reasonably small values (≤ ~10⁵)
 * in the test suite; the double error is far below 1, guaranteeing
 * correct integer truncation. */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;
    /* constant representation of sqrt(5) */
    const double sqrt5 = 2.23606797749979;
    const double phi = (1.0 + sqrt5) / 2.0;   /* (1+√5)/2 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* floor(k * phi) is obtained by truncating the double value */
        idx_t term = (idx_t)( (double)k * phi );
        prod *= term;
    }
    return prod;
}
