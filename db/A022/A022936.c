/*
 * A022936  a(n) = a(n-1) + c(n-1) for n >= 2, a( ) increasing, given a(1)=4;
 *          where c( ) is complement of a( ).
 * https://oeis.org/A022936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 347, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A022936: a(1) = 4; for n ≥ 2,
   a(n) = a(n-1) + b(n-1)  where b is the increasing complement sequence
   (the list of positive integers that are not in A022936). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static cache for the sequence values */
static seq_t *a_cache = NULL;          /* cached sequence values      */
static size_t a_cache_cap = 0;         /* allocated capacity of a_cache */
static idx_t a_len = 0;                /* number of elements currently cached */

/* index into a_cache whose value is ≥ next_missing */
static idx_t present_idx = 0;
static seq_t next_missing = 1;         /* next missing integer (b's value) */
static bool initialized = false;

/* Ensure a_cache can hold at least 'at' (0‑based) element. */
static void ensure_a_cache(size_t at)
{
    if (at < a_cache_cap)
        return;
    size_t new_cap = a_cache_cap ? a_cache_cap << 1 : 16;
    while (new_cap <= at)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_arr)
        exit(EXIT_FAILURE);
    a_cache = new_arr;
    a_cache_cap = new_cap;
}

/* Initialise the cache with a(1)=4. */
static void init_seq(void)
{
    if (initialized)
        return;
    a_cache_cap = 16;
    a_cache = (seq_t *)malloc(a_cache_cap * sizeof(seq_t));
    if (!a_cache)
        exit(EXIT_FAILURE);
    a_cache[0] = 4;      /* a(1) */
    a_len = 1;
    next_missing = 1;
    present_idx = 0;
    initialized = true;
}

/* Return the n‑th term of sequence A022936. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined for n < 1 */
    init_seq();

    if (n <= a_len)
        return a_cache[n - 1];

    /* Generate terms until we reach index n. */
    for (; a_len < n; ++a_len) {
        /* Skip any a values that match the current missing integer. */
        while (present_idx < a_len && a_cache[present_idx] == next_missing) {
            ++present_idx;
            ++next_missing;
        }

        /* Use the current missing integer. */
        seq_t use_missing = next_missing;
        ++next_missing;            /* Prepare for the next iteration. */

        /* Compute the new term. */
        seq_t new_val = a_cache[a_len - 1] + use_missing;

        ensure_a_cache(a_len);     /* a_len is the index for the new value. */
        a_cache[a_len] = new_val;
    }

    return a_cache[n - 1];
}
