/*
 * A079696  Numbers one more than composite numbers.
 * https://oeis.org/A079696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002808(n) + 1
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrime.h"

/* Cache of composite numbers (sequence A002808). */
static seq_t *composites = NULL;
static size_t comp_len = 0;          /* number of cached composites   */
static size_t comp_cap = 0;          /* allocated capacity            */
static idx_t next_candidate = 4;     /* next integer to test for composite */

/* Grow the composites array as needed. */
static void grow_composites(void)
{
    size_t new_cap = comp_cap ? comp_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(composites, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);   /* abort on OOM */
    composites = tmp;
    comp_cap = new_cap;
}

/* Ensure that at least n composites are cached. */
static void ensure_cache(idx_t n)
{
    while (comp_len < n) {
        if (!isPrime(next_candidate)) {   /* use the provided primality test */
            if (comp_len == comp_cap) grow_composites();
            composites[comp_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* behaviour for indices < 1 is undefined */
    ensure_cache(n);
    return composites[n - 1] + 1; /* A002808(n) + 1 */
}
