/*
 * A193433  Sum of the divisors of n^2+1.
 * https://oeis.org/A193433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the divisors of n^2 + 1 (σ(n^2+1))
 * Range:     u8 n <= 12, u16 n <= 186, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static bool primes_initialized = false;
static uint64_t primes[20000];
static size_t prime_count = 0;

/* Simple sieve to fill the primes array up to 10000 */
static void init_primes(void) {
    if (primes_initialized) return;
    bool is_composite[10001] = {0};
    for (uint64_t i = 2; i <= 10000; ++i) {
        if (!is_composite[i]) {
            primes[prime_count++] = i;
            if (i * i <= 10000) {
                for (uint64_t j = i * i; j <= 10000; j += i)
                    is_composite[j] = true;
            }
        }
    }
    primes_initialized = true;
}

/* Compute sigma(n) for a given unsigned number represented in seq_t */
static seq_t sigma_of_seq_t(seq_t n) {
    seq_t result = 1;
    seq_t remaining = n;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        seq_t p_seq = (seq_t)p;
        if (p_seq * p_seq > remaining) break;
        if (remaining % p_seq == 0) {
            seq_t p_pow = 1;
            seq_t sum_p = 1;
            while (remaining % p_seq == 0) {
                remaining /= p_seq;
                p_pow *= p_seq;
                sum_p += p_pow;
            }
            result *= sum_p;
        }
    }
    if (remaining > 1) {
        /* remaining is a prime factor > sqrt(original n) */
        result *= (seq_t)(1 + remaining);
    }
    return result;
}

seq_t a(idx_t n) {
    init_primes();

    /* n may be large; compute n^2 + 1 as seq_t */
    seq_t nn = (seq_t)n;
    seq_t m = nn * nn + 1;

    return sigma_of_seq_t(m);
}
