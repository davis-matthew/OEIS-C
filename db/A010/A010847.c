/*
 * A010847  Number of numbers <= n with a prime factor that does not divide n.
 * https://oeis.org/A010847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - A010846(n) where A010846(n) counts the integers m ≤ n
 *            whose prime factors all divide n
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache of smallest prime factors (spf) */
static uint32_t *global_spf = NULL;
static idx_t spf_limit = 0;

/* Build spf array up to at least limit */
static void init_spf(idx_t limit)
{
    if (spf_limit >= limit)
        return;

    uint32_t *new_spf = (uint32_t *)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    if (!new_spf)
        exit(EXIT_FAILURE);           /* graceful exit on allocation failure */

    for (idx_t i = 0; i <= limit; ++i)
        new_spf[i] = (uint32_t)i;      /* initially spf[i] = i */

    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i)
        if (new_spf[i] == (uint32_t)i)
            for (idx_t j = i * i; j <= limit; j += i)
                if (new_spf[j] == (uint32_t)j)
                    new_spf[j] = (uint32_t)i;

    free(global_spf);
    global_spf = new_spf;
    spf_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined for n=0, return 0 */

    /* Ensure spf array covers n */
    init_spf(n);

    /* Collect distinct prime factors of n */
    uint32_t primes_of_n[64];
    int prime_n_len = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        uint32_t p = global_spf[(size_t)tmp];
        if (prime_n_len == 0 || primes_of_n[prime_n_len - 1] != p)
            primes_of_n[prime_n_len++] = p;
        tmp /= p;
    }

    /* Count numbers m ≤ n that have all prime factors in primes_of_n */
    seq_t count_small = 1;   /* m = 1 always qualifies */
    for (idx_t m = 2; m <= n; ++m) {
        idx_t x = m;
        while (x > 1) {
            uint32_t p = global_spf[(size_t)x];
            bool found = false;
            for (int k = 0; k < prime_n_len; ++k)
                if (primes_of_n[k] == p) { found = true; break; }
            if (!found) break;          /* prime factor not in primes_of_n */
            while (x % p == 0) x /= p;
        }
        if (x == 1)                       /* all prime factors were in the list */
            ++count_small;
    }

    return (seq_t)n - count_small;       /* a(n) = n - A010846(n) */
}
