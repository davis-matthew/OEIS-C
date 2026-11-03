/*
 * A137751  a(n) = number of missing residues in the Fibonacci sequence mod
 *          the n-th prime number.
 * https://oeis.org/A137751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of residues modulo the n‑th prime which do not
 *            occur in the Fibonacci sequence modulo that prime
 * Range:     u8 n <= 54, u16 n <= 6631, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9022 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Fibonacci modulo p -------------- */
static seq_t fib_missing_residues(uint32_t p)
{
    unsigned char *seen = (unsigned char *)calloc((size_t)p, 1);
    if (!seen) return (seq_t)0;      /* allocation failure -> zero */

    size_t seen_count = 0;

    /* Residues 0 and 1 always appear for any prime p >= 2. */
    seen[0] = 1;
    ++seen_count;
    if (p > 1)
    {
        seen[1] = 1;
        ++seen_count;
    }

    seq_t f0 = 0, f1 = 1;
    for (;;)
    {
        seq_t f_next = (f0 + f1) % (seq_t)p;

        /* Record the new residue if it has not been seen yet. */
        if (!seen[(size_t)f_next])
        {
            seen[(size_t)f_next] = 1;
            ++seen_count;
        }

        f0 = f1;
        f1 = f_next;

        /* Period ends when the pair (0,1) repeats. */
        if (f0 == 0 && f1 == 1) break;
    }

    free(seen);
    return (seq_t)p - (seq_t)seen_count;
}

/* ---------- Prime sieve ---------------------------------------- */
static uint32_t *primes = NULL;
static size_t   prime_count = 0;

static void init_primes(void)
{
    static bool done = false;
    if (done) return;

    /* Prime(10000) = 104729. 200 000 is more than enough to hold all
       needed primes. */
    const size_t limit = 200000;

    bool *is_prime = (bool *)malloc(limit + 1);
    if (!is_prime) exit(EXIT_FAILURE);

    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; (size_t)i * i <= limit; ++i)
    {
        if (is_prime[i])
        {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes and allocate array. */
    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++count;

    primes = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[idx++] = (uint32_t)i;

    prime_count = count;
    free(is_prime);
    done = true;
}

static uint32_t nth_prime(idx_t k)
{
    init_primes();
    if (k == 0 || (size_t)k > prime_count) return 0;
    return primes[(size_t)k - 1];
}

/* ---------- Public function -------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* sequence defined from index 1 */
    uint32_t p = nth_prime(n);
    if (p == 0) return 0;           /* out of range, should not happen */
    return fib_missing_residues(p);
}
