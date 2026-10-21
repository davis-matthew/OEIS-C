/*
 * A087428  a(n) = [(p-1)/2 * (q-1)/2] mod p where p = (n+1)th odd prime, q =
 *          n-th odd prime.
 * https://oeis.org/A087428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((p-1)/2 * (q-1)/2) mod p, where p = (n+1)-th odd prime,
 *            q = n-th odd prime
 * Range:     u8 n <= 52, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* -------- Prime list (all primes, 2 at index 0) ---------- */
static uint32_t *prime_list = NULL;   /* primes in ascending order */
static size_t   prime_count = 0;      /* number of primes stored */
static size_t   sieve_limit = 0;      /* largest generated limit */

/* ---- Upper bound for the m‑th prime (m >= 1) ---- */
static size_t estimate_limit(size_t m)
{
    if (m < 6) return 15;                 /* p6 = 13 */
    double dm = (double)m;
    double est = dm * (log(dm) + log(log(dm)));
    return (size_t)(est + 3.0);           /* safety margin */
}

/* ---- Sieve all primes up to 'limit' (inclusive) ---- */
static void generate_primes(size_t limit)
{
    bool *sieve = (bool*)calloc(limit + 1, sizeof(bool));
    if (!sieve) exit(1);                 /* memory failure */

    for (size_t i = 2; i <= limit; ++i) sieve[i] = true;

    for (size_t i = 2; (size_t)i * i <= limit; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = false;

    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++count;

    uint32_t *new_list = (uint32_t*)malloc(count * sizeof(uint32_t));
    if (!new_list) exit(1);

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) new_list[idx++] = (uint32_t)i;

    free(sieve);
    free(prime_list);

    prime_list   = new_list;
    prime_count  = count;
    sieve_limit  = limit;
}

/* ---- Ensure that at least 'full_index' primes exist (zero‑based) ---- */
static void ensure_primes(size_t full_index)
{
    if (prime_count > full_index) return;

    size_t needed_limit = estimate_limit(full_index + 1); /* count is 1‑based */
    if (sieve_limit < needed_limit)
        generate_primes(needed_limit);
}

/* --------- The required sequence function ---------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* We need the (n+1)-th odd prime, which is prime_list[n+1] (since
       prime_list[0] = 2, prime_list[1] = 3 is the 1st odd prime). */
    size_t target_index = (size_t)n + 1;   /* zero‑based index of (n+1)-th odd prime */
    ensure_primes(target_index);          /* guarantees prime_count > target_index */

    uint32_t q = prime_list[(size_t)n];   /* n-th odd prime   */
    uint32_t p = prime_list[(size_t)n + 1]; /* (n+1)-th odd prime */

    seq_t part1 = ((seq_t)p - (seq_t)1) / (seq_t)2;
    seq_t part2 = ((seq_t)q - (seq_t)1) / (seq_t)2;

    /* Multiplication and modulo are defined for seq_t. */
    seq_t prod = part1 * part2 % (seq_t)p;

    return prod;
}
