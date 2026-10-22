/*
 * A000668  Mersenne primes (primes of the form 2^n - 1).
 * https://oeis.org/A000668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, hard
 * Formula:   a(n) = 2^A000043(n) - 1; finds the exponents by searching primes
 *            p, trial-factoring 2^p-1 and then running the Lucas-Lehmer test
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   has_small_factor, lucas_lehmer, pow2_mod, small_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/has_small_factor.h"
#include "../utils/lucas_lehmer.h"
#include "../utils/pow2_mod.h"
#include "../utils/small_prime.h"

static unsigned exps[128];
static idx_t found = 0;
static unsigned next_p = 2;

seq_t a(idx_t n) {
    unsigned p, i;
    seq_t r;
    while (found < n) {
        p = next_p++;
        if (!small_prime(p)) continue;
        if (p > 2 && has_small_factor(p)) continue;
        if (lucas_lehmer(p)) exps[found++] = p;
    }
    p = exps[n - 1];
    r = 1;
    for (i = 0; i < p; i++) r += r;
    return r - 1;
}
