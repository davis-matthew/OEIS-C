/*
 * A204914  Ordered differences of squared primes.
 * https://oeis.org/A204914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = squared_prime(j) – squared_prime(i) * where j is the
 *            smallest index with j*(j-1)/2 ≥ n, and * i = n – (j-1)*(j-2)/2
 *            (1 ≤ i < j). The sequence enumerates * the differences of
 *            squares of primes in ascending order of the * larger prime
 * Range:     u8 n <= 15, u16 n <= 1431, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ---------------------- Prime sieve ---------------------- */

static uint32_t primes[2000];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt > 0)
        return;

    /* Suffices to find the first 2000 primes (≈ 35000 limit). */
    const size_t limit = 35000;
    char *comp = (char *)calloc(limit + 1, 1);
    if (!comp)
        return; /* In a real program, handle allocation failure. */

    for (size_t i = 2; i <= limit && prime_cnt < 2000; ++i) {
        if (!comp[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    comp[j] = 1;
            }
        }
    }
    free(comp);
}

/* ------------------------- a(n) ------------------------- */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* Undefined for n < 1, but return 0 to avoid UB. */

    /* Find the block index j such that j*(j-1)/2 ≥ n. */
    idx_t j = 2;        /* the smallest possible j (prime index). */
    idx_t tri = 1;      /* j*(j-1)/2 for j = 2. */

    while (tri < n) {
        ++j;
        tri += j - 1;
    }

    /* Compute i (1‑based) from n. */
    idx_t prev = tri - (j - 1);         /* triangular number up to j‑1. */
    idx_t i = n - prev;                 /* 1 ≤ i < j.            */

    init_primes();                      /* ensure primes are ready.  */

    /* Retrieve the relevant primes (0‑based indexing). */
    uint32_t pj = primes[(size_t)(j - 1)];
    uint32_t pi = primes[(size_t)(i - 1)];

    /* Compute the difference of squares. */
    uint64_t diff = (uint64_t)pj * pj - (uint64_t)pi * pi;

    return (seq_t)diff;                  /* implicit conversion ok for all builds. */
}
