/*
 * A236968  Number of ordered ways to write n = k + m with k > 0 and m > 0
 *          such that 6*k - 1, 6*k + 1 and k + phi(m) are all prime, where
 *          phi(.) is Euler's totient function.
 * https://oeis.org/A236968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (k,m) with k>0, m>0, k+m=n,
 *            6*k-1, 6*k+1 and k+phi(m) all prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------------------------------------------------------------------
   An integer square‑free helper: exists for prime tests.
   We use a simple trial division, which is fast enough for the bounds
   the sequence requires (n ≤ 10 000 in the test suite).
   --------------------------------------------------------------------- */
static bool is_prime_small(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    if (x % 3 == 0) return x == 3;
    for (uint64_t i = 5; i * i <= x; i += 6)
        if (x % i == 0 || x % (i + 2) == 0) return false;
    return true;
}

/* ---------------------------------------------------------------------
   Sieve of Eratosthenes stored in a static table.
   The table is grown only when a larger limit is requested.
   --------------------------------------------------------------------- */
static bool   *prime_table = NULL;   /* prime_table[i] == true iff i is prime */
static uint64_t prime_limit = 0;     /* maximum value currently sieved, inclusive */

static void ensure_prime_table(uint64_t limit)
{
    if (limit <= prime_limit) return;                      /* already enough */

    /* free old table if any */
    if (prime_table) free(prime_table);

    /* allocate new table; cast to avoid C++ implicit conversion error */
    prime_table = (bool*)calloc(limit + 1, sizeof(bool));
    if (!prime_table) exit(EXIT_FAILURE);                 /* cannot allocate */

    for (uint64_t i = 2; i <= limit; ++i) prime_table[i] = true;

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (prime_table[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                prime_table[j] = false;

    prime_limit = limit;
}

/* ---------------------------------------------------------------------
   Totient table: φ(i) for 0 ≤ i ≤ phi_limit.
   Built once and extended on demand.
   --------------------------------------------------------------------- */
static uint32_t *phi_table = NULL;   /* φ(i) stored as uint32_t (fits φ(10 000) < 2³¹) */
static uint64_t phi_limit = 0;

static void ensure_phi_table(uint64_t limit)
{
    if (limit <= phi_limit) return;                       /* already enough */

    if (phi_table) free(phi_table);

    /* allocate new table; cast to avoid C++ implicit conversion error */
    phi_table = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    if (!phi_table) exit(EXIT_FAILURE);

    phi_table[0] = 0;
    phi_table[1] = 1;
    for (uint64_t i = 2; i <= limit; ++i) phi_table[i] = (uint32_t)i;

    for (uint64_t i = 2; i <= limit; ++i)
        if (phi_table[i] == i) {                         /* i is prime */
            for (uint64_t j = i; j <= limit; j += i)
                phi_table[j] -= phi_table[j] / (uint32_t)i;
        }

    phi_limit = limit;
}

/* ---------------------------------------------------------------------
   Main sequence function.
   --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Convert index to an unsigned 64‑bit integer (it is guaranteed to fit). */
    uint64_t n64 = (uint64_t)n;

    /* Ensure prime sieve covers up to 6*n+1, and φ table up to n. */
    uint64_t prime_needed = 6 * n64 + 1;
    ensure_prime_table(prime_needed);
    ensure_phi_table(n64);

    uint64_t count = 0;

    for (uint64_t k = 1; k < n64; ++k) {
        uint64_t m  = n64 - k;
        uint64_t p1 = 6 * k - 1;
        uint64_t p2 = 6 * k + 1;

        /* Test primality of 6*k-1 and 6*k+1.  All needed values are
           guaranteed to be <= 6*n+1, so the sieve is sufficient. */
        if (!prime_table[p1] || !prime_table[p2]) continue;

        uint32_t phi_m = phi_table[m];
        uint64_t kp = k + phi_m;

        /* kp may exceed prime_limit (unlikely for n≤10 000),
           but we guard against that by testing primality manually. */
        if (kp > prime_limit) {
            if (!is_prime_small(kp)) continue;
        } else {
            if (!prime_table[kp]) continue;
        }

        ++count;
    }

    return (seq_t)count;
}
