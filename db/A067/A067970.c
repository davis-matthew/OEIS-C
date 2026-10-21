/*
 * A067970  First differences of A014076, the odd nonprimes.
 * https://oeis.org/A067970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014076(n+1) - A014076(n) (first differences of odd
 *            nonprimes)
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *odd_nonprimes = NULL;   /* cache of odd nonprimes (1‑based, zero‑based internal) */
static idx_t  odd_nonprimes_count = 0;   /* number of elements currently cached */
static size_t odd_nonprimes_cap   = 0;   /* allocated capacity */

/* Ensure that the cache contains at least 'need' odd nonprimes. */
static void ensure_odd_nonprimes(idx_t need)
{
    if (odd_nonprimes_count >= need) return;

    /* Grow the cache if necessary. */
    if ((size_t)need > odd_nonprimes_cap) {
        size_t new_cap = odd_nonprimes_cap ? odd_nonprimes_cap * 2 : 8;
        while (new_cap < (size_t)need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(odd_nonprimes, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        odd_nonprimes = tmp;
        odd_nonprimes_cap = new_cap;
    }

    /* Start searching from the next odd integer after the last cached value. */
    seq_t candidate;
    if (odd_nonprimes_count == 0) {
        candidate = 1;                     /* 1 is the first odd nonprime */
    } else {
        candidate = odd_nonprimes[odd_nonprimes_count - 1] + (seq_t)2;
    }

    /* Collect odd nonprimes until we have the required amount. */
    while (odd_nonprimes_count < need) {
        if (!is_prime_big(candidate)) {
            odd_nonprimes[odd_nonprimes_count++] = candidate;
        }
        candidate += (seq_t)2;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* undefined for n < 1 */
    ensure_odd_nonprimes(n + 1);             /* need (n)th and (n+1)th odd nonprimes */
    return odd_nonprimes[n] - odd_nonprimes[n - 1];
}
