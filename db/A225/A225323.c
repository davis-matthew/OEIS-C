/*
 * A225323  Numbers n such that 11n is a partition number.
 * https://oeis.org/A225323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A225361(n)/11, where A225361 is the n-th partition number
 *            divisible by 11
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A225323: numbers n such that 11n is a partition number. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* helper to grow a dynamically allocated array */
static void *grow_array(void *arr, size_t *cap, size_t need, size_t elem_size)
{
    if (*cap < need) {
        size_t new_cap = (*cap > 0) ? (*cap << 1) : 16;
        while (new_cap < need) new_cap <<= 1;
        void *tmp = realloc(arr, new_cap * elem_size);
        arr = tmp;
        *cap = new_cap;
    }
    return arr;
}

/* global state for partitions and sequence values */
static seq_t *partitions = NULL;   /* partition numbers p[0], p[1], … */
static size_t part_cap   = 0;      /* capacity of partitions array */
static idx_t  part_next  = 1;      /* next partition index to compute (p[0] already set) */

static seq_t *a_vals    = NULL;    /* a[0], a[1], … (a(n) = p(k)/11) */
static size_t a_cap      = 0;      /* capacity of a_vals array */
static size_t a_count    = 0;      /* number of a-values stored */

/* compute the partition number p[n] using Euler’s pentagonal theorem */
static void compute_partition(idx_t n)
{
    seq_t pos = 0, neg = 0;

    for (idx_t k = 1;; ++k) {
        idx_t pent1 = (k * (3 * k - 1)) / 2;
        if (pent1 > n) break;
        seq_t term = partitions[(size_t)(n - pent1)];
        if (k & 1) pos += term; else neg += term;

        idx_t pent2 = (k * (3 * k + 1)) / 2;
        if (pent2 <= n) {
            term = partitions[(size_t)(n - pent2)];
            if (k & 1) pos += term; else neg += term;
        }
    }

    seq_t p_n = pos - neg;
    partitions[(size_t)n] = p_n;

    if (p_n % (seq_t)11 == 0) {
        seq_t q = p_n / (seq_t)11;
        a_vals = (seq_t *)grow_array(a_vals, &a_cap, a_count + 1, sizeof(seq_t));
        a_vals[a_count++] = q;
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1: a(1) is first term */

    /* initial allocation of partitions array */
    if (!partitions) {
        partitions = (seq_t *)grow_array(partitions, &part_cap, 16, sizeof(seq_t));
        partitions[0] = (seq_t)1;  /* p(0) = 1 */
    }

    /* generate partitions until we have at least n values of the sequence */
    while (a_count < n) {
        if (part_next >= part_cap)
            partitions = (seq_t *)grow_array(partitions, &part_cap, part_next + 1, sizeof(seq_t));
        compute_partition(part_next);
        ++part_next;
    }

    return a_vals[n - 1];
}
