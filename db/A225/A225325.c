/*
 * A225325  Partition numbers of the form 5k.
 * https://oeis.org/A225325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*A217725(n)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 40, u64 n <= 141, u128 n <= 507,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A225325: partition numbers that are multiples of 5 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"

static seq_t *partitions = NULL;          /* partition numbers p[0..] (p[0] = 1) */
static size_t part_cap = 0;
static size_t part_len = 0;

static seq_t *multiples = NULL;           /* partition numbers divisible by 5 */
static size_t multiples_cap = 0;
static size_t multiples_len = 0;

/*
 * Return the n-th partition number that is a multiple of 5.
 * Calls may come in arbitrary order; results are cached.
 */
seq_t a(idx_t n)
{
    /* Below the offset the behavior is irrelevant. */
    if (n == 0) {
        return 0;
    }

    /* Lazily allocate storage for partition numbers. */
    if (partitions == NULL) {
        ensure_capacity(16, &part_cap, (void **)&partitions, sizeof(seq_t));
        partitions[0] = 1;          /* p(0) = 1 */
        part_len = 1;
    }

    /* Lazily allocate storage for the list of multiples of 5. */
    if (multiples == NULL) {
        ensure_capacity(16, &multiples_cap, (void **)&multiples, sizeof(seq_t));
        multiples_len = 0;
    }

    /* Generate partitions until we have at least n multiples. */
    while (multiples_len < (size_t)n) {
        idx_t i = part_len;           /* next partition index to compute */

        /* Grow the partition array if required. */
        if ((size_t)i >= part_cap) {
            size_t new_cap = part_cap ? part_cap * 2 : 16;
            while (new_cap <= (size_t)i) new_cap *= 2;
            ensure_capacity(new_cap, &part_cap, (void **)&partitions, sizeof(seq_t));
        }

        /* Compute p(i) using the generalized pentagonal-number recurrence. */
        seq_t sum = 0;
        for (int k = 1; ; ++k) {
            idx_t kidx = (idx_t)k;
            idx_t three_k = (idx_t)3 * kidx;
            idx_t g1 = (kidx * (three_k - 1)) / 2;
            if (g1 > i) break;
            seq_t term = partitions[i - g1];
            if (k & 1) sum += term;
            else       sum -= term;

            idx_t g2 = (kidx * (three_k + 1)) / 2;
            if (g2 > i) continue;
            term = partitions[i - g2];
            if (k & 1) sum += term;
            else       sum -= term;
        }
        partitions[i] = sum;
        part_len = i + 1;

        /* Record the value if it is divisible by 5. */
        if (sum % 5 == 0) {
            if (multiples_len >= multiples_cap) {
                size_t new_cap = multiples_cap ? multiples_cap * 2 : 16;
                while (new_cap <= multiples_len) new_cap *= 2;
                ensure_capacity(new_cap, &multiples_cap,
                                 (void **)&multiples, sizeof(seq_t));
            }
            multiples[multiples_len++] = sum;
        }
    }

    return multiples[(size_t)(n - 1)];
}
