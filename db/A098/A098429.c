/*
 * A098429  Number of cousin prime pairs (p, p+4) with p <= n.
 * https://oeis.org/A098429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9857, u16 n <= 9857, u32 n <= 9857, u64 n <= 9857,
 *            u128 n <= 9857, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes: a(n) = #{p prime ≤ n | p+4 prime} ("cousin prime count up to n") */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t limit = n + 4;

    /* Allocate and initialize prime flag array */
    bool *prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!prime) return 0;  /* out of memory, return 0 as fallback */
    for (idx_t i = 0; i <= limit; ++i) prime[i] = true;
    prime[0] = prime[1] = false;

    /* Simple Eratosthenes sieve up to limit */
    for (idx_t i = 2; i * i <= limit; ++i)
        if (prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                prime[j] = false;

    /* Count cousin prime pairs */
    seq_t count = 0;
    /* allocate cumulative array up to n */
    seq_t *cum = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!cum) { free(prime); return 0; }
    cum[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (prime[i] && (i + 4 <= limit) && prime[i + 4])
            ++count;
        cum[i] = count;
    }

    seq_t result = cum[n];

    free(prime);
    free(cum);
    return result;
}
