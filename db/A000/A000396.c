/*
 * A000396  Perfect numbers k: k is equal to the sum of the proper divisors of
 *          k.
 * https://oeis.org/A000396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, core, changed
 * Formula:   a(n) = 2^(p-1)*(2^p - 1) where p = A000043(n) is the n-th
 *            Mersenne prime exponent, found by searching primes p with trial
 *            factoring and the Lucas-Lehmer test (assumes there are no odd
 *            perfect numbers)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   pn_is_prime, pn_lucas_lehmer, pn_powmod2, pn_small_factor,
 *            pn_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pn_is_prime.h"
#include "../utils/pn_lucas_lehmer.h"
#include "../utils/pn_powmod2.h"
#include "../utils/pn_small_factor.h"
#include "../utils/pn_width.h"

seq_t a(idx_t n)
{
    static unsigned ex[256];
    static idx_t cnt = 0;
    static unsigned next_p = 2;
    static int width = -1;
    unsigned p;
    seq_t h, m;

    if (n < 1 || n > 256) return 0;
    if (width < 0) width = pn_width();

    while (cnt < n) {
        p = next_p;
        if (width > 0 && 2 * (uint64_t)p - 1 > (uint64_t)width) return 0;
        next_p++;
        if (!pn_is_prime(p)) continue;
        if (p == 2 || (!pn_small_factor(p) && pn_lucas_lehmer(p)))
            ex[cnt++] = p;
    }

    p = ex[n - 1];
    if (width > 0 && 2 * (uint64_t)p - 1 > (uint64_t)width) return 0;
    m = 1;
    m <<= p;
    m -= 1;
    h = 1;
    h <<= (p - 1);
    return h * m;
}
