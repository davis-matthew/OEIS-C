/*
 * A188620  Primes of the form x^3 + y^3 + 1.
 * https://oeis.org/A188620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes of the form x^3 + y^3 + 1, sorted ascending
 * Range:     u8 none, u16 none, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   cmp_seq_t, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../utils/isPrimeGeneric.h"
#include "../utils/cmp_seq_t.h"

static seq_t *sorted   = NULL;   /* sorted primes found so far          */
static size_t sorted_size = 0;   /* number of primes in sorted list     */
static size_t sorted_cap  = 0;   /* capacity of sorted array            */
static seq_t *unsorted   = NULL; /* primes discovered in the current loop */
static size_t unsorted_size = 0; /* size of unsorted array              */
static size_t unsorted_cap  = 0; /* capacity of unsorted array          */
static idx_t  last_processed = 0;/* largest x (or y) that has been processed */

/* Append a value to the unsorted array, growing it if necessary. */
static void append_unsorted(seq_t val)
{
    if (unsorted_size == unsorted_cap) {
        unsorted_cap = (unsorted_cap ? unsorted_cap * 2 : 256);
        seq_t *tmp = (seq_t *)realloc(unsorted, unsorted_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        unsorted = tmp;
    }
    unsorted[unsorted_size++] = val;
}

/* Merge the unsorted primes into the sorted array, sort, and drop duplicates. */
static void merge_and_sort(void)
{
    if (unsorted_size == 0) return;

    if (sorted_size + unsorted_size > sorted_cap) {
        sorted_cap = (sorted_cap ? sorted_cap * 2 : 256);
        while (sorted_cap < sorted_size + unsorted_size)
            sorted_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(sorted, sorted_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        sorted = tmp;
    }

    memcpy(sorted + sorted_size, unsorted, unsorted_size * sizeof(seq_t));
    sorted_size += unsorted_size;
    unsorted_size = 0;          /* reset for next round */

    qsort(sorted, sorted_size, sizeof(seq_t), cmp_seq_t);

    /* Remove duplicates in place. */
    size_t uniq = 0;
    for (size_t i = 0; i < sorted_size; ++i) {
        if (uniq == 0 || sorted[i] != sorted[uniq - 1])
            sorted[uniq++] = sorted[i];
    }
    sorted_size = uniq;
}

/* Generate primes until we have at least n of them. */
static void generate_up_to(idx_t n)
{
    while (sorted_size < n) {
        idx_t new_limit = (last_processed == 0) ? 100 : last_processed * 2;

        for (idx_t x = 1; x <= new_limit; ++x) {
            for (idx_t y = x; y <= new_limit; ++y) {
                if (x <= last_processed && y <= last_processed) continue;

                seq_t xx = (seq_t)x;
                seq_t yy = (seq_t)y;
                seq_t val = xx * xx * xx + yy * yy * yy + (seq_t)1;

                if (isPrimeGeneric(val))
                    append_unsorted(val);
            }
        }

        merge_and_sort();
        last_processed = new_limit;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, but safe fallback */
    generate_up_to(n);
    return sorted[n - 1];
}
