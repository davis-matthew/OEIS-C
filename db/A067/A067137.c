/*
 * A067137  a(n) is the smallest number m such that n^(1/m) = 1.(leading
 *          zeros)n...
 * https://oeis.org/A067137
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smallest integer m such that the decimal expansion
 *            of n^(1/m) has the form 1.(0…0 n …), i.e. the first few digits
 *            after the decimal point consist of some number of zeros followed
 *            immediately by the decimal digits of n
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/digit_count.h"

static long double pow10i[25];   /* 10^i for i up to 24 */

static void init_pow10(void) {
    int i;
    pow10i[0] = 1.0L;
    for (i = 1; i < 25; ++i) pow10i[i] = pow10i[i-1] * 10.0L;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* value below offset is irrelevant */

    if (!pow10i[0]) init_pow10();       /* lazy static init */

    /* number of digits in n */
    uint64_t k = digit_count((uint64_t)n);

    /* iterate over m until condition satisfied */
    for (uint64_t m = 2; ; ++m) {       /* m=1 never works, so start at 2 */
        long double root = pow((long double)n, 1.0L / (long double)m);
        long double t = root - 1.0L;    /* fractional part */

        /* Try increasing number of leading zeros (p - k) */
        for (int p = (int)k; p < 25; ++p) {
            long double val = t * pow10i[p];
            uint64_t v = (uint64_t)floor(val + 1e-12L); /* guard against rounding */

            if (v == n) {              /* found suitable m */
                return (seq_t)m;
            }
            /* once the value exceeds n+1, further p will not help */
            if (val > (long double)(n + 1)) break;
        }
    }
}
