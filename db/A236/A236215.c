/*
 * A236215  Sum of the ninth powers of the first n primes.
 * https://oeis.org/A236215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} prime(k)^9
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 30, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 999 (2026-11-03)
 * Helpers:   ipow, mem, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "../utils/ipow.h"
#include "../utils/sieve_first_n_primes.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* static cache for cumulative sums up to max requested n */
    static seq_t *cumsum = NULL;
    static size_t cap = 0;     /* allocated capacity for cumsum */
    static idx_t cached_n = 0; /* number of terms currently cached */

    /* If we already have the term, return it immediately */
    if (n <= cached_n)
        return cumsum[n];

    /* Ensure the cache array is large enough */
    grow_seq((seq_t **)&cumsum, &cap, (size_t)n + 1); /* need indices 0..n */

    /* First time: initialise the zero term */
    if (cached_n == 0)
        cumsum[0] = (seq_t)0;

    /* Compute primes up to n and fill missing cumulative sums */
    uint32_t *primes = NULL;
    size_t prime_cnt = 0;
    sieve_first_n_primes(n, &primes, &prime_cnt);

    for (idx_t i = cached_n; i < n; ++i) {
        seq_t p   = (seq_t)primes[i];
        seq_t p9  = ipow(p, (seq_t)9);
        cumsum[i+1] = cumsum[i] + p9;
    }

    /* Clean up temporary prime array */
    free(primes);

    /* Update the cache length */
    cached_n = n;

    return cumsum[n];
}
