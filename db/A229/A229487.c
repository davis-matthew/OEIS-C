/*
 * A229487  Conjectured greatest number that converges to prime(n) under the
 *          iteration x -> phi(x) + 1, where phi is Euler's totient function.
 * https://oeis.org/A229487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 210, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the greatest integer that eventually reaches the n‑th prime
   under the iteration f(x) = φ(x) + 1.  The value is obtained by
   exhaustive forward iteration of all integers up to a fixed upper
   bound (10 000 000).  For each prime p in that range the maximum
   pre‑image found by the enumeration is stored and returned. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_X_LIMIT 10000000U   /* sieved range for forward iteration   */

/* Static storage for values that are initialised once. */
static bool init_done = false;
static unsigned int *phi;       /* φ values for 0 … MAX_X_LIMIT          */
static unsigned char *is_comp;  /* composite marker                      */
static unsigned int *prime_index;/* index of prime in primes list (0 = none) */
static unsigned int *primes;    /* primes up to MAX_X_LIMIT              */
static size_t prime_count;      /* number of primes found                */
static seq_t *best;             /* best[x] = largest number reaching
                                   prime number primes[x]  (1‑based)       */

/* ------------------------------------------------------------------ */
/*  Linear sieve that simultaneously computes φ(n) for 0 ≤ n ≤ L.     */
/* ------------------------------------------------------------------ */
static void compute_phi_and_primes(void)
{
    unsigned int L = MAX_X_LIMIT;
    phi = (unsigned int *)malloc((L + 1) * sizeof(unsigned int));
    is_comp = (unsigned char *)calloc(L + 1, sizeof(unsigned char));
    prime_index = (unsigned int *)calloc(L + 1, sizeof(unsigned int));

    /* initialisation */
    phi[1] = 1;
    primes = (unsigned int *)malloc((L / 10 + 1000) * sizeof(unsigned int)); /* rough reserve */
    prime_count = 0;

    for (unsigned int i = 2; i <= L; ++i) {
        if (!is_comp[i]) {                 /* i is prime */
            primes[prime_count++] = i;
            phi[i] = i - 1;
        }
        for (size_t j = 0; j < prime_count; ++j) {
            unsigned int p = primes[j];
            unsigned long long v = (unsigned long long)p * i;
            if (v > L) break;
            unsigned int vi = (unsigned int)v;
            is_comp[vi] = 1;
            if (i % p == 0) {
                phi[vi] = phi[i] * p;
                break;
            } else {
                phi[vi] = phi[i] * (p - 1);
            }
        }
    }

    /* map each prime value to its 1‑based index in *primes */
    for (size_t i = 0; i < prime_count; ++i) {
        prime_index[primes[i]] = (unsigned int)(i + 1);
    }
}

/* ------------------------------------------------------------------ */
/*  Enumerate all numbers up to MAX_X_LIMIT, follow the iteration
    until a prime is reached, and keep the maximum pre‑image for each
    prime.                                                            */
/* ------------------------------------------------------------------ */
static void init(void)
{
    if (init_done) return;
    compute_phi_and_primes();

    best = (seq_t *)calloc(prime_count, sizeof(seq_t));
    for (unsigned int x = 2; x <= MAX_X_LIMIT; ++x) {
        unsigned int v = x;
        while (phi[v] != v - 1) {          /* stop when v is prime   */
            v = phi[v] + 1;
        }
        unsigned int idx = prime_index[v] - 1;   /* 0‑based index */
        if (idx < prime_count) {
            if ((seq_t)x > best[idx]) best[idx] = (seq_t)x;
        }
    }
    init_done = true;
}

/* ------------------------------------------------------------------ */
/*  Public required function.                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!init_done) init();

    if (n < 1 || (size_t)n > prime_count) return 0;
    /* n is 1‑based index into primes array */
    return best[(size_t)n - 1];
}
