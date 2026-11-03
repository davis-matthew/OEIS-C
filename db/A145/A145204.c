/*
 * A145204  Numbers whose representation in base 3 (A007089) ends in an odd
 *          number of zeros.
 * https://oeis.org/A145204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0 * a(n) = 3 * m (n > 1) * where m is the (n-1)-th
 *            positive integer whose number of * trailing zeros in base‑3
 *            (i.e. its 3‑adic valuation) is even. * In other words, m has the
 *            form 3^(2k) * u with u not divisible by 3. * The function walks
 *            through integers sequentially, counting those * wi...
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9968 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* helper: true iff m has an even number of factors of 3 */
static bool even_v3(seq_t m)
{
    idx_t e = 0;
    while (m % (seq_t)3 == 0) {
        m /= (seq_t)3;
        e++;
    }
    return ((e & 1ULL) == 0);
}

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0, also handles n==0 gracefully */
        return 0;

    idx_t target = n - 1;   /* we need the (n-1)-th member of A007417 */
    seq_t m = 1;            /* candidate numbers for A007417 */
    idx_t count = 0;        /* how many have been found so far */

    while (count < target) {
        if (even_v3(m))    /* m qualifies */
            count++;
        if (count < target)
            m++;           /* advance to next candidate */
    }

    return (seq_t)3 * m;    /* a(n) = 3 * m */
}
