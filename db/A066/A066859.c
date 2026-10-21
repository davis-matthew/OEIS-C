/*
 * A066859  Product of sums of divisors and non-divisors.
 * https://oeis.org/A066859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) * (n*(n+1)/2 - sigma(n)) where sigma(n) = sum of
 *            positive divisors of n
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 1439, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t sigma_of(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_n = 0;

    if (n <= max_n) return cache[n];

    /* enlarge cache */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) exit(1);      /* abort on memory failure */
    cache = new_cache;

    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                sum += (seq_t)d;
                idx_t other = i / d;
                if (other != d) sum += (seq_t)other;
            }
        }
        cache[i] = sum;
    }
    max_n = n;
    return cache[n];
}

seq_t a(idx_t n)
{
    seq_t sig = sigma_of(n);
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;
    return sig * (tri - sig);
}
