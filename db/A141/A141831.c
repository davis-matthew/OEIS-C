/*
 * A141831  Primes not in sequence A141830.
 * https://oeis.org/A141831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes p where not every integer k ≤ q‑p divides p‑1
   (q is the next prime after p).  The nth term is returned with
   1‑based indexing. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/next_prime.h"

static seq_t cache[200000];      /* sufficient for n ≈ 10 000 */
static size_t cache_used = 0;    /* number of terms stored so far */
static uint64_t last_checked = 2;/* prime last inspected */

/* Return the n-th term of the sequence (1‑based).  For n==0 returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* Grow cache until it contains at least n terms. */
    while (cache_used < (size_t)n) {
        uint64_t p = next_prime(last_checked);   /* next prime after last_checked */
        uint64_t q = next_prime(p);              /* prime immediately after p */
        uint64_t gap = q - p;                    /* q – p */

        /* Check if all k ≤ gap divide p‑1. */
        bool all_divide = true;
        uint64_t pm1 = p - 1;
        for (uint64_t k = 1; k <= gap; ++k) {
            if (pm1 % k != 0) {
                all_divide = false;
                break;
            }
        }

        if (!all_divide) {
            cache[cache_used++] = (seq_t)p;
        }

        last_checked = p; /* prepare for next iteration */
    }

    return cache[n - 1];
}
