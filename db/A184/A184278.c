/*
 * A184278  Number of distinct n X n toroidal 0..2 arrays.
 * https://oeis.org/A184278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n^2)) * Sum_{c|n} Sum_{d|n} phi(c) * phi(d) *
 *            3^(n^2/lcm(c,d))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/big_pow.h"

static idx_t gcd_idx_t(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n == 0 return 0. */
    if (n == 0)
        return 0;

    /* Dynamic array for divisors. The number of divisors of n <= 10000 is
       well below 256, so a small static array suffices. */
    idx_t divs[256];
    idx_t dcnt = 0;

    /* Compute the integer square root of n. */
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;

    for (idx_t i = 1; i <= r; ++i) {
        if (n % i == 0) {
            divs[dcnt++] = i;
            idx_t j = n / i;
            if (j != i && dcnt < 256)
                divs[dcnt++] = j;
        }
    }

    seq_t sum = 0;
    idx_t n2 = n * n;          /* n^2 fits in idx_t for n <= 10,000 */

    for (idx_t ic = 0; ic < dcnt; ++ic) {
        idx_t c = divs[ic];
        seq_t phi_c = phi(c);
        for (idx_t id = 0; id < dcnt; ++id) {
            idx_t d = divs[id];
            seq_t phi_d = phi(d);

            idx_t g = gcd_idx_t(c, d);
            idx_t lcm = (c / g) * d;          /* lcm(c,d) fits in idx_t */
            idx_t exp = n2 / lcm;             /* exponent for 3^... */

            seq_t term = big_pow((seq_t)3, (seq_t)exp);
            term *= phi_c;
            term *= phi_d;
            sum += term;
        }
    }

    /* Divide by n^2. The division is exact. */
    return sum / (seq_t)n2;
}
