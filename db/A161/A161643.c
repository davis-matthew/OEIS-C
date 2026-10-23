/*
 * A161643  Sum of all numbers from the smallest up to the largest prime
 *          factor of the n-th composite.
 * https://oeis.org/A161643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = sum of all integers from the smallest to the largest
 *            prime factor of the n-th composite number. a(n) =
 *            Σ_{k=spf(n)}^{lpf(n)} k, where spf is the smallest prime factor
 *            and lpf is the largest prime factor of the n-th composite (OEIS
 *            A161643)
 * Range:     u8 n <= 30, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   _smallest_prime_factor, is_prime, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/_smallest_prime_factor.h"
#include "../utils/largest_prime_factor.h"

/* Static storage for the list of composite numbers computed so far. */
static uint64_t *composites = NULL;
static size_t composites_len = 0;
static size_t composites_cap = 0;
static uint64_t last_checked = 3; /* largest number checked for primality */

/* Ensure the composites array has room for at least one more entry. */
static void grow_composites(void)
{
    size_t new_cap = composites_cap ? composites_cap * 2 : 4;
    uint64_t *tmp = (uint64_t *)realloc(composites, new_cap * sizeof(uint64_t));
    composites = tmp;
    composites_cap = new_cap;
}

/* Add a new composite number to the storage. */
static void add_composite(uint64_t val)
{
    if (composites_len == composites_cap)
        grow_composites();
    composites[composites_len++] = val;
}

/* Compute the n-th composite number (n >= 1), extending the cache if necessary. */
static uint64_t nth_composite(idx_t n)
{
    if (composites_len < (size_t)n) {
        for (;;) {
            ++last_checked;
            if (!is_prime((idx_t)last_checked))
                add_composite(last_checked);
            if (composites_len >= (size_t)n)
                break;
        }
    }
    return composites[n - 1];
}

seq_t a(idx_t n)
{
    /* Find the n-th composite number. */
    uint64_t c = nth_composite(n);

    /* Obtain its smallest and largest prime factors. */
    uint64_t spf = _smallest_prime_factor(c);
    uint64_t lpf = largest_prime_factor(c);

    /* Compute sum of consecutive integers from spf to lpf. */
    seq_t sp = (seq_t)spf;
    seq_t lp = (seq_t)lpf;
    seq_t diff = (seq_t)(lpf - spf + 1);          /* number of terms */
    seq_t sum = (sp + lp) * diff / (seq_t)2;      /* arithmetic series sum */

    return sum;
}
