/*
 * A087680  Numbers n such that n + 4 and n - 4 are both prime.
 * https://oeis.org/A087680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th integer m such that m-4 and m+4 are both prime
 *            Computation: Builds a sieve of Eratosthenes up to a growing
 *            limit, scans for numbers whose 4‑offset neighbors are both
 *            prime, and stores them in a static array for fast retrieval
 * Range:     u8 n <= 15, u16 n <= 893, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

static seq_t *found = NULL;            /* array of already found terms */
static size_t found_cap = 0;           /* current capacity of 'found'      */
static size_t found_cnt = 0;           /* number of terms stored          */
static idx_t  current_limit = 0;       /* maximum number examined so far   */

/* Append a value to the 'found' array, resizing if necessary. */
static void append_found(seq_t value) {
    if (found_cnt == found_cap) {
        size_t new_cap = found_cap ? found_cap * 2 : 128;
        seq_t *tmp = (seq_t *)realloc(found, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        found = tmp;
        found_cap = new_cap;
    }
    found[found_cnt++] = value;
}

/* Generate a prime sieve up to 'limit' and scan the new range for terms. */
static void expand(idx_t new_limit) {
    /* Allocate sieve array */
    bool *isprime = (bool *)malloc((new_limit + 1) * sizeof(bool));
    if (!isprime) abort();
    memset(isprime, true, (new_limit + 1) * sizeof(bool));
    isprime[0] = isprime[1] = false;

    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (isprime[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i) {
                isprime[j] = false;
            }
        }
    }

    /* Scan numbers from current_limit + 1 up to new_limit - 4 */
    idx_t start = current_limit + 1;
    if (start < 5) start = 5;          /* skip values where i-4 < 2 */
    for (idx_t i = start; i + 4 <= new_limit; ++i) {
        if (isprime[i - 4] && isprime[i + 4]) {
            append_found((seq_t)i);
        }
    }

    free(isprime);
    current_limit = new_limit;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* offset is 1, but guard against zero */

    while (found_cnt < n) {
        idx_t new_limit;
        if (current_limit == 0) {
            new_limit = 1000000;        /* start with a reasonable size */
        } else {
            new_limit = current_limit * 2;  /* double each time */
        }
        expand(new_limit);
    }
    return found[n - 1];
}
