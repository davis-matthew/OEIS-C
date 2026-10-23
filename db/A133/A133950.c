/*
 * A133950  a(n) = the number of "isolated divisors" of n(n+1)/2. A positive
 *          divisor k of n is isolated if neither k-1 nor k+1 divides n.
 * https://oeis.org/A133950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of isolated divisors of n(n+1)/2. A divisor d
 *            of N = n(n+1)/2 is isolated if N % (d-1) != 0 and N % (d+1) != 0
 *            (treating d-1 = 0 as not dividing)
 * Range:     u8 n <= 22, u16 n <= 728, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _enumerate_divs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_enumerate_divs.h"

seq_t a(idx_t n)
{
    /* Compute N = n(n+1)/2 with seq_t arithmetic. */
    seq_t N = ((seq_t)n * ((seq_t)n + 1)) / 2;

    /* Factor N using simple trial division. */
    uint64_t primes[64];
    int exps[64];
    int cnt = 0;
    uint64_t t = (uint64_t)N;   /* N fits in 64‑bit for all test values */

    if (t % 2 == 0) {
        primes[cnt] = 2;
        exps[cnt] = 0;
        while (t % 2 == 0) {
            t /= 2;
            exps[cnt]++;
        }
        cnt++;
    }

    for (uint64_t d = 3; d * d <= t; d += 2) {
        if (t % d == 0) {
            primes[cnt] = d;
            exps[cnt] = 0;
            while (t % d == 0) {
                t /= d;
                exps[cnt]++;
            }
            cnt++;
        }
    }

    if (t > 1) {
        primes[cnt] = t;
        exps[cnt] = 1;
        cnt++;
    }

    /* Enumerate all divisors and count the isolated ones. */
    seq_t iso_count = 0;
    _enumerate_divs(0, 1, primes, exps, cnt, N, &iso_count);
    return iso_count;
}
