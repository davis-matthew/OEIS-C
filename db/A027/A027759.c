/*
 * A027759  Numerator of Sum_{p prime, p-1|n} 1/p.
 * https://oeis.org/A027759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the sum \sum_{p prime, p-1|n} 1/p. The sum
 *            equals (∑_{p} (∏_{q} q)/p) / (∏_{q} q), so its reduced numerator
 *            is simply N = ∑_{p} (P / p) where P = ∏_{p} p
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 179, u64 n <= 539,
 *            u128 n <= 3779, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Global sieve state, shared by all calls. */
static bool *prime = NULL;       /* prime[i] == true  ⇔  i is prime */
static size_t prime_limit = 0;   /* current maximum index in prime[] */

/* Build (or extend) a prime sieve up to 'limit'.  The array has length
   limit+1, indices 0..limit.  It is re‑allocated only when a larger
   limit is requested. */
static void ensure_sieve(idx_t limit)
{
    if (prime && limit <= prime_limit)
        return;  /* already sufficient */

    /* Allocate a fresh sieve. */
    bool *new_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!new_prime)
        return;  /* unlikely, but leave old sieve untouched */

    for (size_t i = 0; i <= limit; ++i)
        new_prime[i] = true;
    if (limit >= 0) new_prime[0] = false;
    if (limit >= 1) new_prime[1] = false;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (new_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                new_prime[j] = false;
        }
    }

    /* Replace old sieve (if any). */
    free(prime);
    prime = new_prime;
    prime_limit = limit;
}

/* Fast primality test using the global sieve; if the number is larger
   than the current sieve limit, a simple trial division is performed. */
static bool is_prime_cached(idx_t x)
{
    if (x <= prime_limit)
        return prime[x];
    if (x < 2)
        return false;
    if ((x & 1) == 0)
        return x == 2;
    for (idx_t d = 3; d * d <= x; d += 2)
        if (x % d == 0)
            return false;
    return true;
}

/* Main function computing the nth term of OEIS A027759. */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined by the sequence; return 0 for safety */
        return (seq_t)0;

    /* Ensure primes up to n+1 are known (p-1 ≤ n ⇒ p ≤ n+1). */
    ensure_sieve(n + 1);

    /* Collect all primes p with p-1 | n.  n has at most a few dozen such primes. */
    uint64_t primes[256];
    size_t pcnt = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t p1 = d + 1;
            if (p1 <= n + 1 && is_prime_cached(p1))
                primes[pcnt++] = (uint64_t)p1;

            idx_t d2 = n / d;
            if (d2 != d) {
                idx_t p2 = d2 + 1;
                if (p2 <= n + 1 && is_prime_cached(p2))
                    primes[pcnt++] = (uint64_t)p2;
            }
        }
    }

    /* Compute product P = ∏ primes[i]. */
    seq_t P = (seq_t)1;
    for (size_t i = 0; i < pcnt; ++i)
        P *= (seq_t)primes[i];

    /* Compute numerator N = ∑ (P / primes[i]).  All divisions are exact. */
    seq_t N = (seq_t)0;
    for (size_t i = 0; i < pcnt; ++i)
        N += P / (seq_t)primes[i];

    return N;
}
