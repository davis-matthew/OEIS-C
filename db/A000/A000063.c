/*
 * A000063  Symmetrical dissections of an n-gon.
 * https://oeis.org/A000063
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(floor(n/2) - 1) - Catalan(n/4 - 1) - Catalan(n/6
 *            - 1), where Catalan(x) = 0 for noninteger x; Catalan(i+1) =
 *            Catalan(i)*(4i+2)/(i+2)
 * Range:     u8 n <= 15, u16 n <= 25, u32 n <= 41, u64 n <= 75, u128 n <= 141,
 *            big any n
 * Helpers:   gcd_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_ull.h"

seq_t a(idx_t n)
{
    unsigned long long m = (unsigned long long)(n / 2) - 1;
    bool has4 = (n % 4 == 0);
    bool has6 = (n % 6 == 0);
    unsigned long long k4 = has4 ? (unsigned long long)(n / 4) - 1 : 0;
    unsigned long long k6 = has6 ? (unsigned long long)(n / 6) - 1 : 0;

    seq_t c = 1;          /* Catalan(0) */
    seq_t c4 = 0, c6 = 0;
    unsigned long long i;

    for (i = 0;; i++) {
        if (has4 && i == k4) c4 = c;
        if (has6 && i == k6) c6 = c;
        if (i == m) break;
        {
            unsigned long long num = 4 * i + 2;
            unsigned long long den = i + 2;
            unsigned long long g = gcd_ull(num, den);
            num /= g;
            den /= g;
            c /= den;     /* exact: den is coprime to num and divides c*num */
            c *= num;
        }
    }

    return c - c4 - c6;
}
