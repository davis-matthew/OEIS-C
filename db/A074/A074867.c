/*
 * A074867  a(n) = M(a(n-1)) + M(a(n-2)) where a(1)=a(2)=1 and M(k) is the
 *          product of the digits of k in base 10.
 * https://oeis.org/A074867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = M(a(n-1)) + M(a(n-2)) , a(1)=a(2)=1 * where M(k) is the
 *            product of the decimal digits of k. * For all terms a(n) < 20,
 *            hence M(k)=k (mod 10). Therefore * a(n) = (a(n-1) mod 10) +
 *            (a(n-2) mod 10). The sequence * becomes periodic with least
 *            period 60. * * The function precomputes the first 6...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t period[61];   /* 1‑based index, period[1] … period[60] */

    if (!init) {
        /* initial values */
        period[1] = 1;
        period[2] = 1;
        for (idx_t i = 3; i <= 60; ++i) {
            /* use only the ones digit of the previous two terms */
            seq_t lo1 = period[i - 1] % (seq_t)10;
            seq_t lo2 = period[i - 2] % (seq_t)10;
            period[i] = lo1 + lo2;  /* result < 20, fits in seq_t */
        }
        init = true;
    }

    /* sequence is periodic with period 60, offset 1 */
    idx_t pos = ((n - 1) % 60) + 1;
    return period[pos];
}
