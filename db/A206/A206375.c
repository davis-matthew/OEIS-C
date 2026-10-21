/*
 * A206375  Vinogradov's constants arising in enumeration of solutions to
 *          Waring's problem in the evil numbers (A001969).
 * https://oeis.org/A206375
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n for 3 ≤ n ≤ 10; for n > 10, a(n) = 2 * floor( n^2 *
 *            (log(n) + log(log(n)) + 4) )
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* values below offset (n < 3) are irrelevant; return 0 */
    if (n < 3) {
        return (seq_t)0;
    }

    if (n <= 10) {
        /* compute 2^n using left shift */
        return ((seq_t)1) << n;
    }

    /* compute the real expression for n > 10 */
    double dn = (double)n;
    double val = dn * dn * (log(dn) + log(log(dn)) + 4.0);

    /* floor the double value; add a tiny epsilon to guard against rounding errors */
    unsigned long long flo = (unsigned long long)floor(val + 1e-12);

    /* multiply by 2 */
    return (seq_t)(2 * flo);
}
