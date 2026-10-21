/*
 * A095247  Least k such that the concatenation a(1), a(2), a(3), ..., a(n-1),
 *          a(n) is divisible by n if and only if n is not a prime.
 * https://oeis.org/A095247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, uned
 * Formula:   a(n) = Least k such that concatenation of a(1)..a(n) is
 *            divisible by * n if and only if n is not prime
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper function: number of decimal digits of a positive integer. */
static inline int digits_uint64(uint64_t x)
{
    int d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

/* Simple sieve to test primality up to the current bound.  */
static bool *prime_sieve = NULL;
static idx_t prime_sieve_size = 0;

static void ensure_sieve(idx_t n)
{
    if (n <= prime_sieve_size) return;
    bool *new_sieve = (bool*)malloc((n + 1) * sizeof(bool));
    if (!new_sieve) return;           /* memory failure handling omitted */
    for (idx_t i = 0; i <= n; ++i) new_sieve[i] = true;
    if (n >= 0) new_sieve[0] = false;
    if (n >= 1) new_sieve[1] = false;
    for (idx_t i = 2; i * i <= n; ++i) {
        if (new_sieve[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                new_sieve[j] = false;
        }
    }
    if (prime_sieve) free(prime_sieve);
    prime_sieve = new_sieve;
    prime_sieve_size = n;
}

static inline bool is_prime(idx_t x)
{
    if (x < 2) return false;
    ensure_sieve(x);
    return prime_sieve[x];
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[1..] */
    static idx_t cache_size = 0;  /* current maximum n computed */

    if (n == 0) return 0; /* undefined, but safe */

    /* Expand cache if necessary.  */
    if (n > cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;  /* memory failure handling omitted */
        cache = new_cache;
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1) {                     /* a(1) = 1 by definition */
                cache[i] = 1;
                continue;
            }

            /* Special shortcut: if n equals 10^k, 2*10^k or 5*10^k, a(n)=n. */
            bool shortcut = false;
            if (n % 10 == 0) {
                idx_t tmp = n;
                while (tmp % 10 == 0) tmp /= 10;
                if (tmp == 1 || tmp == 2 || tmp == 5) shortcut = true;
            }
            if (shortcut) {
                cache[i] = (seq_t)i;
                continue;
            }

            bool need_divisible = !is_prime(i);  /* composite => need divisible */

            /* Compute C_{i-1} modulo i. */
            uint64_t rem = 0;
            for (idx_t j = 1; j < i; ++j) {
                uint64_t val = (uint64_t)cache[j];
                int d = digits_uint64(val);
                uint64_t pow10 = 1 % i;
                for (int k = 0; k < d; ++k) pow10 = (pow10 * 10) % i;
                rem = (rem * pow10 + val) % i;
            }

            /* Find smallest candidate. */
            uint64_t cand = 1;
            while (1) {
                int d = digits_uint64(cand);
                uint64_t pow10 = 1 % i;
                for (int k = 0; k < d; ++k) pow10 = (pow10 * 10) % i;
                uint64_t mod_val = (rem * pow10 + cand) % i;
                bool divisible = (mod_val == 0);
                if (divisible == need_divisible) {
                    cache[i] = (seq_t)cand;
                    break;
                }
                ++cand;
            }
        }
        cache_size = n;
    }

    return cache[n];
}
