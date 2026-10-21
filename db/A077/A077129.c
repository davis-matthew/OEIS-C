/*
 * A077129  Smallest number which is relatively prime to all the numbers
 *          between successive odd primes.
 * https://oeis.org/A077129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest prime that is relatively prime to every
 *            integer between the n‑th and (n+1)‑st successive odd primes p_n
 *            and p_{n+1} (offset 0)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define PRIME_LIMIT 2000000          /* upper bound for sieve */
#define PRIME_ARRAY_SIZE 200000      /* enough to hold all primes below PRIME_LIMIT */

/* Basic 64‑bit GCD */
static inline unsigned long long gcd64(unsigned long long a, unsigned long long b)
{
    while (b) { unsigned long long t = a % b; a = b; b = t; }
    return a;
}

/* Storage for odd primes (>2) */
static uint64_t primes[PRIME_ARRAY_SIZE];
static size_t prime_count = 0;
static int primes_initialized = 0;

/* Sieve of Eratosthenes to fill 'primes' array */
static void init_primes(void)
{
    if (primes_initialized) return;
    char *iscomp = (char*)calloc(PRIME_LIMIT + 1, 1);
    for (int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!iscomp[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                iscomp[j] = 1;
    for (int i = 3; i <= PRIME_LIMIT; i += 2)
        if (!iscomp[i]) {
            if (prime_count < PRIME_ARRAY_SIZE)
                primes[prime_count++] = (uint64_t)i;
        }
    free(iscomp);
    primes_initialized = 1;
}

/* ---- Main sequence function ---- */
seq_t a(idx_t n)
{
    init_primes();
    /* safety check – if n is beyond our pre‑computed range, return 0 */
    if ((size_t)n + 1 >= prime_count) return (seq_t)0;

    uint64_t p = primes[(size_t)n];
    uint64_t q = primes[(size_t)n + 1];

    /* test all odd primes ≤ p (including 3) */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t cand = primes[i];
        if (cand > p) break;          /* all remaining primes are larger */
        int ok = 1;
        for (uint64_t m = p + 1; m < q; ++m)
            if (gcd64(cand, m) != 1ULL) { ok = 0; break; }
        if (ok) return (seq_t)cand;
    }

    /* unlikely fallback – search for the next prime beyond our table */
    uint64_t cand = primes[prime_count - 1] + 2;
    while (1) {

        /* primality test by trial division using known primes */
        int is_prime = 1;
        for (size_t j = 0; j < prime_count; ++j) {
            uint64_t pr = primes[j];
            if (pr * pr > cand) break;
            if (cand % pr == 0) { is_prime = 0; break; }
        }
        if (!is_prime) { cand += 2; continue; }

        int ok = 1;
        for (uint64_t m = p + 1; m < q; ++m)
            if (gcd64(cand, m) != 1ULL) { ok = 0; break; }
        if (ok) return (seq_t)cand;

        cand += 2;
    }
}
