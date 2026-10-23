/*
 * A165295  The larger member of a prime pair (p,p + 1000).
 * https://oeis.org/A165295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth larger member of a prime pair (p, p+1000).
   The sequence contains primes q such that q-1000 is also prime;
   a(n) is the n‑th such q, starting with a(1) = 1013. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *matches = NULL;       /* stored larger members */
static size_t matches_len = 0;
static size_t matches_cap = 0;
static idx_t last_checked = 2;      /* next integer to test */

static void grow_matches(void)
{
    if (matches_cap == 0) {
        matches_cap = 8;
        matches = (seq_t*)malloc(matches_cap * sizeof(seq_t));
    } else {
        matches_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(matches, matches_cap * sizeof(seq_t));
        if (!tmp) return;           /* OOM – not expected in tests */
        matches = tmp;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not used – defined for safety */

    /* Generate results until we have at least n entries */
    while (matches_len < n) {
        if (last_checked <= 1000) last_checked = 1001;   /* avoid underflow */

        if (is_prime(last_checked) && is_prime(last_checked - 1000)) {
            if (matches_len == matches_cap) grow_matches();
            matches[matches_len++] = (seq_t)last_checked;
        }
        ++last_checked;
    }

    return matches[n - 1];
}
