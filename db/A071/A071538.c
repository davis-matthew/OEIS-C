/*
 * A071538  Number of twin prime pairs (p, p+2) with p <= n.
 * https://oeis.org/A071538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of twin prime pairs (p, p+2) with p ≤ n (OEIS
 *            A071538)
 * Range:     u8 n <= 9308, u16 n <= 9308, u32 n <= 9308, u64 n <= 9308,
 *            u128 n <= 9308, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *twin_counts = NULL;   /* prefix counts of twins up to the last computed limit */
static idx_t  computed_limit = 0;   /* highest n for which twin_counts[0..n] is valid */

/* Recompute the twin‑prime prefix array up to 'limit'.
   The sieve is built up to limit+2 so that pairs starting at
   limit can be detected. */
static void recompute_up_to(idx_t limit)
{
    /* allocate prime flag array: indices 0 … limit+2 inclusive */
    const idx_t sieve_size = limit + 3;
    uint8_t *is_prime = (uint8_t *)malloc(sieve_size * sizeof(uint8_t));
    if (!is_prime) return;            /* memory error – leave old cache untouched */

    for (idx_t i = 0; i < sieve_size; ++i) is_prime[i] = 1;
    if (sieve_size > 0) is_prime[0] = 0;
    if (sieve_size > 1) is_prime[1] = 0;

    /* Eratosthenes sieve up to sqrt(limit+2) */
    idx_t root = 0;
    while ((root + 1) * (root + 1) <= limit + 2) ++root;
    for (idx_t p = 2; p <= root; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m < sieve_size; m += p) is_prime[m] = 0;
        }
    }

    /* build prefix array of twin counts: twin_counts[i] = #pairs with smaller member ≤ i */
    seq_t *new_counts = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!new_counts) {
        free(is_prime);
        return;                      /* memory error – leave old cache untouched */
    }

    seq_t count = 0;
    for (idx_t i = 0; i <= limit; ++i) {
        if (is_prime[i] && i + 2 <= limit + 2 && is_prime[i + 2]) ++count;
        new_counts[i] = count;
    }

    /* replace old cache */
    if (twin_counts) free(twin_counts);
    twin_counts = new_counts;
    computed_limit = limit;

    free(is_prime);
}

/* Main sequence function, callable in any order */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* a(0) not defined, return 0 */

    if (n > computed_limit)                /* expand cache if needed */
        recompute_up_to(n);

    if (n <= computed_limit) return twin_counts[n];
    /* should never reach here */
    return 0;
}
