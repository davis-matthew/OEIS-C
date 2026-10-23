/*
 * A142217  Primes congruent to 20 mod 41.
 * https://oeis.org/A142217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 158, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to 20 (mod 41) – OEIS A142217. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime.h"

static seq_t *primes_congr = NULL;   /* array of primes ≡ 20 (mod 41) */
static size_t cnt_congr    = 0;      /* number of cached primes */
static size_t cap_congr    = 0;      /* allocated capacity */
static uint64_t last_candidate = 0;  /* last tested candidate */

/* Append a prime to the cache, resizing if necessary. */
static void append_prime(seq_t p)
{
    if (cnt_congr >= cap_congr) {
        size_t new_cap = cap_congr ? cap_congr * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(primes_congr, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE); /* out of memory */
        primes_congr = tmp;
        cap_congr = new_cap;
    }
    primes_congr[cnt_congr++] = p;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n < 1, safe guard */

    /* Generate more primes until we have at least n of them. */
    if (cnt_congr < (size_t)n) {
        if (last_candidate < 61ULL) last_candidate = 61ULL;

        for (; cnt_congr < (size_t)n; last_candidate += 41ULL) {
            uint64_t cand = last_candidate;
            if (is_prime((idx_t)cand)) {
                append_prime((seq_t)cand);
            }
        }
    }

    return primes_congr[n - 1];
}
