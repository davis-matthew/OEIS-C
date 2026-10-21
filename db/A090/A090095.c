/*
 * A090095  a(n) is the smallest composite number coprime to n, n+1, n+2, n+3,
 *          n+4 and n+5.
 * https://oeis.org/A090095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (least prime p not dividing any of n, n+1, ..., n+5)^2
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Sieve bound – large enough for n up to at least 10 000 */
static const int PRIME_LIMIT = 200000;
/* Number of primes <= PRIME_LIMIT is 17984, buffer a bit larger */
static int primes[20000];
static size_t primeCnt = 0;
static bool primes_initialized = false;

/* Perform one‑time sieve of Eratosthenes */
static void init_primes(void)
{
    if (primes_initialized)
        return;
    bool *sieve = (bool*)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!sieve)
        exit(1);                /* insufficient memory – terminate */
    for (int i = 0; i <= PRIME_LIMIT; i++)
        sieve[i] = true;
    sieve[0] = sieve[1] = false;

    int limit = 1;
    while ((long long)limit * limit <= PRIME_LIMIT)
        ++limit;

    for (int i = 2; i <= limit; i++) {
        if (sieve[i]) {
            for (long long j = 1LL * i * i; j <= PRIME_LIMIT; j += i)
                sieve[(int)j] = false;
        }
    }

    for (int i = 2; i <= PRIME_LIMIT; i++)
        if (sieve[i])
            primes[primeCnt++] = i;

    free(sieve);
    primes_initialized = true;
}

/* Smallest prime not dividing any of n..n+5 */
seq_t a(idx_t n)
{
    init_primes();

    /* Search primes from the pre‑computed list */
    for (size_t i = 0; i < primeCnt; ++i) {
        int p = primes[i];
        bool ok = true;
        for (int k = 0; k < 6; ++k) {
            if (((unsigned long long)n + k) % p == 0) {
                ok = false;
                break;
            }
        }
        if (ok) {
            seq_t pval = (seq_t)p;
            return pval * pval;
        }
    }

    /* Fallback: search beyond the sieve bound */
    int candidate = PRIME_LIMIT + 1;
    while (1) {
        bool isprime = true;
        for (int d = 2; (long long)d * d <= candidate; ++d) {
            if (candidate % d == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            bool ok = true;
            for (int k = 0; k < 6; ++k) {
                if (((unsigned long long)n + k) % candidate == 0) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                seq_t pval = (seq_t)candidate;
                return pval * pval;
            }
        }
        ++candidate;
    }
}
