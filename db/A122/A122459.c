/*
 * A122459  Triangular array of sums of two odd primes.
 * https://oeis.org/A122459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum of two odd primes p(k) + p(2*n+1-k) read row‑wise
 *            from the triangular array where p(1)=3, p(2)=5, ... are the odd
 *            primes
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* ---------- prime cache ---------- */
static uint32_t *primes = NULL;    /* primes[0] = 3, primes[1] = 5, ... */
static idx_t prime_count = 0;      /* number of primes stored */
static idx_t prime_capacity = 0;   /* allocated capacity */
static uint64_t next_candidate = 3;

/* Ensure that at least `need` odd primes are in the cache. */
static void ensure_primes(idx_t need)
{
    if (prime_count >= need)
        return;

    if (prime_capacity == 0) {
        prime_capacity = 16;
        primes = (uint32_t *)malloc(prime_capacity * sizeof(uint32_t));
    }

    while (prime_count < need) {
        bool is_prime = true;
        uint64_t c = next_candidate;
        uint64_t limit = (uint64_t) sqrtl((long double)c);

        for (idx_t i = 0; i < prime_count && primes[i] <= limit; ++i) {
            if (c % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (prime_count == prime_capacity) {
                prime_capacity *= 2;
                primes = (uint32_t *)realloc(primes, prime_capacity * sizeof(uint32_t));
            }
            primes[prime_count++] = (uint32_t)c;
        }
        next_candidate += 2;   /* skip even numbers */
    }
}

/* Return the k-th odd prime, 1‑based. */
static uint32_t get_prime(idx_t k)
{
    ensure_primes(k);
    return primes[k - 1];
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)  /* out of range, but return 0 safely */
        return (seq_t)0;

    /* Find the row r such that r*(r+1)/2 >= n */
    long double ld = (long double)n;
    idx_t r = (idx_t)((sqrtl(1.0L + 8.0L * ld) - 1.0L) / 2.0L);
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Position k within the row (1‑based) */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;          /* 1 <= k <= r */

    /* Corresponding second index */
    idx_t k2 = 2 * r + 1 - k;

    uint32_t p1 = get_prime(k);
    uint32_t p2 = get_prime(k2);

    return (seq_t)p1 + (seq_t)p2;
}
