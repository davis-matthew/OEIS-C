/*
 * A137898  Limiting sequence when we start with positive integers (A000027)
 *          and at step n >= 1 add to the term at position n + a(n) the value
 *          of the term at position n+a(n)+1.
 * https://oeis.org/A137898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence A137898.
 * Start with a[n] = n for all positive n.
 * For each n ≥ 1, let k = n + a[n]; then
 *     a[k] += a[k+1].
 * After all such steps the value at position n stabilises;
 * that is a(n).  This implementation evaluates the sequence
 * iteratively, memoising all values that have been processed.
 * The routine works for any sequence type seq_t and index type
 * idx_t supplied by the build system. */

static void _ensure_cap(idx_t idx, seq_t **arr, size_t *cap) {
    if ((size_t)idx >= *cap) {
        size_t new_cap = *cap ? (*cap << 1) : 16;
        while (new_cap <= (size_t)idx)
            new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        for (size_t i = *cap; i < new_cap; ++i)
            (*arr)[i] = (seq_t)i;          /* initialisation a[i] = i */
        *cap = new_cap;
    }
}

seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t *table = NULL;
    static size_t table_cap = 0;
    static idx_t processed = 0;          /* last n for which the step
                                            a[n + a[n]] += a[n + a[n] + 1]
                                            has been executed */

    /* first call: initialise table */
    if (table == NULL) {
        table_cap = 16;
        table = (seq_t *)malloc(table_cap * sizeof(seq_t));
        for (size_t i = 0; i < table_cap; ++i)
            table[i] = (seq_t)i;          /* a[i] = i for all i >= 0 */
        table[0] = (seq_t)0;
    }

    /* make sure indices up to n are allocated */
    _ensure_cap(n, &table, &table_cap);

    /* perform all pending steps up to n (inclusive) */
    while (processed < n) {
        ++processed;                       /* current n being processed */
        seq_t an = table[processed];        /* current a[processed] */
        idx_t k = processed + (idx_t)an;   /* n + a[n] */

        /* allocate indices k and k+1 if needed */
        _ensure_cap(k + 1, &table, &table_cap);

        /* apply the update a[k] += a[k+1] */
        table[k] += table[k + 1];
    }

    return table[n];
}
