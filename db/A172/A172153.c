/*
 * A172153  Where records occur for number of partitions of n into two
 *          nonsquares.
 * https://oeis.org/A172153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A172153 by brute‑force searching for record
 * partition sums: a(n) is the smallest m such that the number of
 * unordered pairs of nonsquares summing to m sets a new record. */

/* --------------------------------------------------------------------
 * Internal state
 * -------------------------------------------------------------------- */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* is_nonsquare[i] == 1 if i is not a perfect square, 0 otherwise. */
static unsigned char *is_nonsquare = NULL;
static size_t nonsquare_cap = 0;          /* current capacity of is_nonsquare */

/* Prefetched record data */
static idx_t current_max_n = 0;            /* next integer to process */
static uint64_t current_max_count = 0;    /* maximum partition count seen so far */
static idx_t *record_values = NULL;       /* array of record m values */
static size_t record_cap = 0;              /* capacity of record_values */
static size_t record_len = 0;              /* number of records stored */

/* --------------------------------------------------------------------
 * Helpers
 * -------------------------------------------------------------------- */
static void
ensure_nonsquare_up_to(idx_t limit)
{
    if ((size_t)(limit + 1) <= nonsquare_cap)
        return;                                     /* already enough */

    size_t new_cap = (size_t)(limit + 1);
    unsigned char *new_arr = (unsigned char *)malloc(new_cap);
    if (!new_arr)
        abort();

    /* initialise all to 1 (nonnonquares) */
    for (size_t i = 0; i < new_cap; ++i)
        new_arr[i] = 1;

    /* mark perfect squares as 0 */
    idx_t k = 1;
    while ((idx_t)k * k <= limit) {
        new_arr[(size_t)(k * k)] = 0;
        ++k;
    }

    free(is_nonsquare);
    is_nonsquare = new_arr;
    nonsquare_cap = new_cap;
}

static void
append_record(idx_t value)
{
    if (record_len == record_cap) {
        size_t new_cap = record_cap == 0 ? 8 : record_cap * 2;
        idx_t *new_arr = (idx_t *)realloc(record_values, new_cap * sizeof(idx_t));
        if (!new_arr)
            abort();
        record_values = new_arr;
        record_cap = new_cap;
    }
    record_values[record_len++] = value;
}

/* Count unordered partitions of n into two nonsquares. */
static inline uint64_t
count_partitions(idx_t n)
{
    uint64_t cnt = 0;
    idx_t half = n / 2;
    for (idx_t a = 1; a <= half; ++a) {
        if (is_nonsquare[(size_t)a] &&
            is_nonsquare[(size_t)(n - a)])
            ++cnt;
    }
    return cnt;
}

/* Keep computed records at least up to desired_num entries. */
static void
ensure_records_up_to(size_t desired_num)
{
    ensure_nonsquare_up_to(current_max_n);

    while (record_len < desired_num) {
        uint64_t cnt = count_partitions(current_max_n);
        if (record_len == 0 || cnt > current_max_count) {
            append_record(current_max_n);
            current_max_count = cnt;
        }
        ++current_max_n;
        ensure_nonsquare_up_to(current_max_n);
    }
}

/* --------------------------------------------------------------------
 * Sequence function
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* behaviour below offset is irrelevant */

    size_t needed = (size_t)n;
    ensure_records_up_to(needed);
    return (seq_t)(record_values[n - 1]); /* convert to seq_t type */
}
