/*
 * A141100  Number of unordered pairs of odd composite numbers that sum to 2n.
 * https://oeis.org/A141100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of unordered pairs of odd composite numbers that sum to
 *            2n. * The function sieves odd composites up to 2n and counts
 *            pairs using a * two‑pointer scan over the sorted list
 * Range:     u8 n <= 165, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Helpers for arbitrary‑precision seq_t */
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

/* Static state for memoised sieve and composite list. */
static idx_t   current_limit     = 0;          /* largest number already sieved */
static bool    *prime_flags      = NULL;       /* prime flag array */
static seq_t   *odd_composites   = NULL;       /* sorted odd composites */
static size_t  composites_count = 0;           /* number of odd composites */

/* Resize and rebuild tables to include all numbers up to `need_limit`. */
static void
ensure_tables(idx_t need_limit)
{
    if (need_limit <= current_limit)
        return;               /* already sufficient */

    /* Allocate (or grow) the prime flag array. */
    bool *new_flags = (bool *)realloc(prime_flags,
                         (size_t)(need_limit + 1) * sizeof(bool));
    if (!new_flags) {
        exit(EXIT_FAILURE);
    }
    prime_flags = new_flags;

    /* Initialise all entries to prime; then sieve. */
    for (idx_t i = 0; i <= need_limit; ++i)
        prime_flags[i] = true;
    if (need_limit >= 0) prime_flags[0] = false;
    if (need_limit >= 1) prime_flags[1] = false;
    for (idx_t p = 2; p * p <= need_limit; ++p) {
        if (prime_flags[p]) {
            for (idx_t m = p * p; m <= need_limit; m += p)
                prime_flags[m] = false;
        }
    }

    /* Count how many odd composites will be stored. */
    size_t tmp = 0;
    for (idx_t x = 9; x <= need_limit; x += 2) {
        if (!prime_flags[x])
            ++tmp;
    }

    /* Allocate (or replace) the odd composites array. */
    seq_t *new_arr = seq_alloc_new_array(tmp);
    if (!new_arr) {
        exit(EXIT_FAILURE);
    }
    if (odd_composites)
        free_seq_array(odd_composites);
    odd_composites = new_arr;

    /* Fill the array. */
    size_t pos = 0;
    for (idx_t x = 9; x <= need_limit; x += 2) {
        if (!prime_flags[x])
            odd_composites[pos++] = (seq_t)x;
    }
    composites_count = pos;
    current_limit = need_limit;
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;              /* undefined for n < 1 */

    /* Build data structures up to 2n. */
    ensure_tables(2 * n);

    if (composites_count == 0)
        return (seq_t)0;              /* no odd composites available */

    idx_t target = 2 * n;
    seq_t count  = 0;

    /* Two‑pointer search over the sorted odd composites. */
    size_t i = 0;
    size_t j = composites_count - 1;

    while (i <= j) {
        idx_t sum = (idx_t)odd_composites[i] + (idx_t)odd_composites[j];
        if (sum == target) {
            ++count;
            ++i;
            if (j == 0)
                break;                /* prevent underflow */
            --j;
        } else if (sum < target) {
            ++i;
        } else {                       /* sum > target */
            if (j == 0)
                break;
            --j;
        }
    }
    return count;
}
