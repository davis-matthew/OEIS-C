/*
 * A119805  a(1) = 1. For m >= 0 and 1 <= k <= 2^m, a(2^m +k) = number of
 *          earlier terms of the sequence which equal k.
 * https://oeis.org/A119805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A119805: a(1) = 1.
   For n > 1, let m = ⌊log₂(n − 1)⌋, k = n − 2ᵐ.
   Then a(n) = number of earlier terms equal to k. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* compute A119805(n) for any n ≥ 1, memoising across calls */
seq_t a(idx_t n)
{
    /* static arrays that remember the computed values */
    static seq_t *seq = NULL;            /* seq[0] is a(1) */
    static size_t seq_len = 0;           /* current number of stored terms */

    static seq_t *counts = NULL;         /* counts[v] = number of times value v has occurred */
    static size_t counts_len = 0;        /* current capacity of counts array */

    /* safety guard – not used for valid OEIS offset */
    if (n == 0) return 0;

    /* if term already computed, return it */
    if (n <= seq_len) return seq[(size_t)n - 1];

    /* --- enlarge seq array as needed -------------------------------------- */
    {
        size_t needed = (size_t)n;
        if (!seq) {
            seq = (seq_t *)calloc(needed, sizeof(seq_t));
            if (!seq) abort();
        } else if (needed > seq_len) {
            seq = (seq_t *)realloc(seq, needed * sizeof(seq_t));
            if (!seq) abort();
        }
    }

    /* --- initialise counts array for the first few values ------------------ */
    if (!counts) {
        counts_len = 2; /* we will need indices 0 and 1 at least */
        counts = (seq_t *)calloc(counts_len, sizeof(seq_t));
        if (!counts) abort();
    }

    /* --- compute new terms from seq_len+1 up to n -------------------------- */
    for (idx_t idx = seq_len + 1; (size_t)idx <= n; ++idx) {
        seq_t value;

        if (idx == 1) {
            value = 1;                /* a(1) = 1 */
        } else {
            /* largest power of two ≤ idx‑1 */
            idx_t m = 63 - __builtin_clzll((uint64_t)(idx - 1));
            idx_t power = (idx_t)1ULL << m;
            idx_t k = idx - power;    /* 1 ≤ k ≤ 2ᵐ */

            /* enlarge counts if needed for index k */
            if ((size_t)k >= counts_len) {
                size_t new_len = k + 1;
                while (new_len <= (size_t)k) new_len <<= 1;   /* double until enough */

                seq_t *tmp = (seq_t *)realloc(counts, new_len * sizeof(seq_t));
                if (!tmp) abort();
                /* initialise new entries to 0 */
                for (size_t j = counts_len; j < new_len; ++j) tmp[j] = 0;
                counts = tmp;
                counts_len = new_len;
            }

            value = counts[(size_t)k];
        }

        /* store the computed value */
        seq[(size_t)idx - 1] = value;

        /* ensure counts array can hold index = value (the number of occurrences) */
        if ((size_t)value >= counts_len) {
            size_t new_len = (size_t)value + 1;
            while (new_len <= (size_t)value) new_len <<= 1;

            seq_t *tmp = (seq_t *)realloc(counts, new_len * sizeof(seq_t));
            if (!tmp) abort();
            for (size_t j = counts_len; j < new_len; ++j) tmp[j] = 0;
            counts = tmp;
            counts_len = new_len;
        }

        /* increment the count of this value for future accesses */
        ++counts[(size_t)value];
    }

    /* update the stored length and return the requested term */
    seq_len = (size_t)n;
    return seq[(size_t)n - 1];
}
