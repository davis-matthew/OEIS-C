/*
 * A082736  LCM of n-th group of terms in A074147.
 * https://oeis.org/A082736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM of the next n integers of parity equal to n, starting
 *            from: if n is odd: 2*((n-1)/2)^2 + 1 (odd numbers) if n is even:
 *            2*((n/2)*(n/2-1)+1) (even numbers) This implements the
 *            description from OEIS A082736
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* Build a list of all primes up to 10000.  This guarantees
   coverage of sqrt(max_value) for n ≤ 10000. */
#define PRIME_LIMIT 10000
#define MAX_PRIME_CNT 1229   /* number of primes ≤ 10000 */

static uint32_t prime_list[MAX_PRIME_CNT];
static size_t prime_cnt = 0;
static bool primes_built = false;

static void init_primes(void)
{
    bool sieve[PRIME_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = false;
    prime_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) prime_list[prime_cnt++] = (uint32_t)i;
}

seq_t a(idx_t n)
{
    /* Ensure primes are available */
    if (!primes_built) { init_primes(); primes_built = true; }

    uint64_t first;
    if ((n & 1ULL) == 0) {                 /* even n */
        uint64_t m = n / 2ULL;              /* m ≥ 1 */
        first = 2ULL * (m * (m - 1ULL) + 1ULL);
    } else {                                /* odd n */
        uint64_t m = (n - 1ULL) / 2ULL;     /* m ≥ 0 */
        first = 2ULL * m * m + 1ULL;
    }

    /* Store maximum exponent for each prime encountered in the group. */
    static uint32_t prime_used[2048];
    static unsigned int exp_used[2048];
    size_t used = 0;

    for (uint64_t off = 0; off < n; ++off) {
        uint64_t val = first + 2ULL * off;
        uint64_t temp = val;

        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            uint32_t p = prime_list[pi];
            uint64_t p2 = (uint64_t)p * p;
            if (p2 > temp) break;
            unsigned int cnt = 0;
            while ((temp % p) == 0) { temp /= p; ++cnt; }
            if (cnt > 0) {
                /* Find or add prime p */
                size_t idx = 0;
                while (idx < used && prime_used[idx] != p) ++idx;
                if (idx < used) {
                    if (cnt > exp_used[idx]) exp_used[idx] = cnt;
                } else {
                    prime_used[used] = p;
                    exp_used[used] = cnt;
                    ++used;
                }
            }
        }

        if (temp > 1) {   /* remaining prime factor > sqrt */
            uint32_t p = (uint32_t)temp;
            unsigned int cnt = 1;
            size_t idx = 0;
            while (idx < used && prime_used[idx] != p) ++idx;
            if (idx < used) {
                if (cnt > exp_used[idx]) exp_used[idx] = cnt;
            } else {
                prime_used[used] = p;
                exp_used[used] = cnt;
                ++used;
            }
        }
    }

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < used; ++i) {
        seq_t p = (seq_t)prime_used[i];
        seq_t p_pow = seq_pow(p, (idx_t)exp_used[i]);
        result = seq_mul(result, p_pow);
    }
    return result;
}
