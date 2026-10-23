/*
 * A145537  a(n) is the number of numbers removed in each step of
 *          Eratosthenes's sieve for 10!.
 * https://oeis.org/A145537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(n) = number of numbers removed in step n of Eratosthenes sieve
 *            for 10!
 * Range:     u8 none, u16 none, u32 n <= 291, u64 n <= 291, u128 n <= 291,
 *            big any n
 * Helpers:   _sqrt_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/_sqrt_uint64.h"

static bool _initialized = false;
static seq_t *_steps = NULL;
static size_t _steps_len = 0;

seq_t a(idx_t n)
{
    if (!_initialized) {
        const uint64_t N = 3628800ULL; /* 10! */

        /* Classic Eratosthenes sieve to list primes up to N */
        char *is_prime = (char *)malloc(N + 1);
        for (uint64_t i = 0; i <= N; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;
        uint64_t limit = _sqrt_uint64(N);
        for (uint64_t i = 2; i <= limit; ++i) {
            if (is_prime[i]) {
                for (uint64_t j = i * i; j <= N; j += i)
                    is_prime[j] = 0;
            }
        }

        /* Collect all primes in order */
        uint64_t *primes = NULL;
        size_t primes_cap = 0;
        size_t primes_cnt = 0;
        for (uint64_t i = 2; i <= N; ++i) {
            if (is_prime[i]) {
                if (primes_cnt >= primes_cap) {
                    primes_cap = primes_cap ? primes_cap * 2 : 16;
                    primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                }
                primes[primes_cnt++] = i;
            }
        }
        free(is_prime);

        /* Prepare storage for removal counts per prime */
        _steps_len = primes_cnt;
        _steps = (seq_t *)malloc(_steps_len * sizeof(seq_t));

        /* Mark composites as they are crossed off */
        char *removed = (char *)calloc(N + 1, 1);  /* 0 = not removed, 1 = removed */
        for (size_t idx = 0; idx < primes_cnt; ++idx) {
            uint64_t p = primes[idx];
            uint64_t start = p * p;
            seq_t cnt = 0;
            if (start <= N) {
                for (uint64_t m = start; m <= N; m += p) {
                    if (!removed[m]) {
                        removed[m] = 1;
                        ++cnt;
                    }
                }
            }
            _steps[idx] = cnt;
        }
        free(removed);
        free(primes);

        _initialized = true;
    }

    if (n == 0 || n > (idx_t)_steps_len)
        return 0;
    return _steps[n - 1];
}
