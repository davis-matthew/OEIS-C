/*
 * A093999  Primes of the form abs(floor(k*log(k)-sqrt(k*(k+2*Pi)/Pi))).
 * https://oeis.org/A093999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = the n-th prime of the form abs(floor(k*log(k) -
 *            sqrt(k*(k+2*Pi)/Pi)))
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_len = 0;
    static idx_t last_k = 0;
    static const long double PI = 3.14159265358979323846264338327950288419716939937510L;

    while (primes_len < n) {
        ++last_k;
        long double kld = (long double)last_k;
        long double val = kld * logl(kld) - sqrtl(kld * (kld + 2.0L * PI) / PI);
        long long floor_val = (long long)floorl(val);
        uint64_t cand64 = (floor_val < 0) ? (uint64_t)(-floor_val) : (uint64_t)floor_val;
        if (cand64 > 1) {
            seq_t cand = (seq_t)cand64;
            if (is_prime_big(cand)) {
                if (primes_len == primes_cap) {
                    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                    primes = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_len++] = cand;
            }
        }
    }
    return primes[n - 1];
}
