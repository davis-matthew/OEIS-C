/*
 * A046308  Numbers that are divisible by exactly 7 primes counting
 *          multiplicity.
 * https://oeis.org/A046308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1880, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_list
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046308: Numbers that are divisible by exactly 7 primes counting multiplicity. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/compute_list.h"

static seq_t *seq = NULL;          /* array of 7‑almost primes */
static size_t seq_len = 0;         /* number of elements in seq */
static idx_t cur_limit = 0;        /* current sieve limit */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */

    /* If never initialized, start with a moderate limit. */
    if (!seq) {
        cur_limit = 1 << 20;       /* start with 1,048,576 */
        compute_list(cur_limit, &seq, &seq_len);
    }

    /* Extend the list until it contains at least n terms. */
    while (n > seq_len) {
        cur_limit <<= 1;           /* double the limit each time */
        seq_t *new_seq = NULL;
        size_t new_len = 0;
        compute_list(cur_limit, &new_seq, &new_len);
        free(seq);
        seq = new_seq;
        seq_len = new_len;
    }

    return seq[n - 1];              /* 1‑based indexing */
}
