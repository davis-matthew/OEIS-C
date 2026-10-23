/*
 * A103654  Primes which are the average of two successive semiprimes.
 * https://oeis.org/A103654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 856, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_semiprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: scans integers, keeps track of successive semiprimes and
 * whenever their average is a prime it is appended to the sequence.
 * The function memoizes the results so subsequent calls are O(1). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/is_semiprime_uint64.h"

/* seq_t and idx_t are provided by the build system. */
extern void *malloc(size_t);
extern void *realloc(void *, size_t);
extern void free(void *);

static seq_t *seq = NULL;          /* array of found terms */
static size_t seq_cap = 0;         /* allocated capacity of seq */
static idx_t seq_len = 0;          /* number of terms stored */

/* State for the incremental scan */
static uint64_t scan_curr = 2;    /* next number to test */
static uint64_t prev_semiprime = 0; /* last semiprime encountered */

/* Helper: grow seq array if needed */
static void seq_grow(void) {
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed, abort */
            free(seq);
            seq = NULL;
            seq_cap = 0;
            seq_len = 0;
            return;
        }
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* Add a term to the sequence */
static void seq_add(seq_t v) {
    seq_grow();
    if (seq) seq[seq_len++] = v;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* ensure n >= 1 */
    if (n < 1) return 0;

    /* compute terms until we have at least n of them */
    while (seq_len < n) {
        uint64_t x = scan_curr++;

        if (is_semiprime_uint64(x)) {
            if (prev_semiprime != 0) {
                uint64_t q = prev_semiprime;
                uint64_t r = x;
                /* (q + r) must be even to have an integer average */
                if (((q + r) & 1ULL) == 0ULL) {
                    uint64_t mid = (q + r) >> 1;
                    if (is_prime(mid)) {
                        seq_add((seq_t)mid);
                    }
                }
            }
            prev_semiprime = x;
        }
    }

    /* seq_len >= n now */
    return seq[n - 1];
}
