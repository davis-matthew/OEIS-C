/*
 * A078892  Numbers n such that phi(n) - 1 is prime, where phi is Euler's
 *          totient function (A000010).
 * https://oeis.org/A078892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 133, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A078892: numbers n such that φ(n) – 1 is prime.  
   The sequence is built by sieving φ(n) up to a fixed limit,
   collecting every n with φ(n)-1 a prime.  This gives a quick lookup
   for any n ≥ 1 up to the pre‑computed quantity (≈ 2·10⁶).  */

#include <stdbool.h>
#include <stdlib.h>

static const idx_t LIMIT = 2000000ULL;          /* pre‑compute enough terms */

/* Cached data */
static idx_t *phi_cache = NULL;   /* φ values */
static bool   *prime_flag = NULL; /* primality flag for numbers ≤ LIMIT */
static seq_t  *seq_cache  = NULL; /* sequence terms */
static size_t seq_len = 0;        /* number of terms stored */

/* Build the arrays only once. */
static void build_cached_sequence(void)
{
    if (phi_cache != NULL)                /* already built */
        return;

    /* Allocate containers */
    phi_cache  = (idx_t*)malloc((size_t)(LIMIT + 1) * sizeof(idx_t));
    if (!phi_cache) abort();

    prime_flag = (bool*)malloc((size_t)(LIMIT + 1) * sizeof(bool));
    if (!prime_flag) abort();

    /* Initialise φ[i] = i, prime_flag[i] = false */
    for (idx_t i = 0; i <= LIMIT; ++i) {
        phi_cache[i] = i;
        prime_flag[i] = false;
    }

    /* Sieve φ and identify primes up to LIMIT */
    for (idx_t p = 2; p <= LIMIT; ++p) {
        if (phi_cache[p] == p) {                 /* p is prime */
            prime_flag[p] = true;
            for (idx_t j = p; j <= LIMIT; j += p) {
                phi_cache[j] -= phi_cache[j] / p;
            }
        }
    }

    /* Allocate sequence cache (LOTS of space for safety) */
    seq_cache = (seq_t*)malloc((size_t)LIMIT * sizeof(seq_t));
    if (!seq_cache) abort();

    /* Collect terms */
    size_t sz = 0;
    for (idx_t n = 1; n <= LIMIT; ++n) {
        idx_t m = phi_cache[n];
        if (m >= 2) {
            idx_t t = m - 1;
            if (t <= LIMIT && prime_flag[t]) {
                seq_cache[sz++] = (seq_t)n;
            }
        }
    }
    seq_len = sz;
}

/* External function required by the challenge */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined below offset */

    build_cached_sequence();              /* ensure data is ready */

    if (n <= seq_len)
        return seq_cache[n-1];

    /* If the requested index exceeds pre‑computed terms,
       return 0 (the caller is not expected to ask for such n). */
    return (seq_t)0;
}
