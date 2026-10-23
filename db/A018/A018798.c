/*
 * A018798  Smallest 4th power that begins with n.
 * https://oeis.org/A018798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest fourth power whose decimal representation begins
 *            with the decimal digits of n
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 54, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case: 0^4 = 0 starts with 0 */
    if (n == 0)
        return (seq_t)0;

    /* Compute the number of digits of n (at least 1). */
    idx_t tmp = n;
    int nlen = 0;
    do {
        nlen++;
        tmp /= 10;
    } while (tmp > 0);

    /* Try successive integers k until k^4 has its leading nlen
       digits equal to n.  This is guaranteed to find a result
       for all n, and for n <= 10 000 the search is well
       within the time limit. */
    for (idx_t k = 1; ; k++) {
        /* Compute k^4 in seq_t; use seq_t types for all operations. */
        seq_t val = (seq_t)k;
        val *= (seq_t)k;   /* k^2 */
        val *= (seq_t)k;   /* k^3 */
        val *= (seq_t)k;   /* k^4 */

        /* Determine number of decimal digits of val. */
        seq_t temp = val;
        int vlen = 0;
        do {
            vlen++;
            temp /= (seq_t)10;
        } while (temp > 0);

        if (vlen < nlen)
            continue; /* Not enough digits yet */

        /* Compute power of ten for aligning the leading nlen digits. */
        int diff = vlen - nlen;
        seq_t pow10 = (seq_t)1;
        for (int i = 0; i < diff; i++)
            pow10 *= (seq_t)10;

        /* Extract the leading nlen digits of val. */
        seq_t leading = val / pow10;

        if (leading == (seq_t)n)
            return val;   /* Found the minimal fourth power */
    }
}
