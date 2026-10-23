/*
 * A165416  Irregular array read by rows: The n-th row contains those distinct
 *          positive integers that each, when written in binary, occurs as a
 *          substring in binary n.
 * https://oeis.org/A165416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabf, look
 * Formula:   a(n) is the n‑th element of the irregular array read by rows, *
 *            where the k‑th row consists of all distinct positive integers
 *            whose * binary representation occurs as a contiguous substring
 *            of the binary * representation of k (0 is omitted). The rows are
 *            concatenated left‑to‑right
 * Range:     u8 n <= 3129, u16 n <= 7836, u32 n <= 7836, u64 n <= 7836,
 *            u128 n <= 7836, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_log2_uint64.h"

/* Ensure that the dynamic array *arr has capacity at least 'needed' slots. */
static void grow_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 32 : *cap;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) return;         /* allocation failure – caller cannot recover */
    *arr = tmp;
    *cap = new_cap;
}

/* Main sequence retrieval function. */
seq_t a(idx_t n)
{
    /* Static storage for the flattened array. */
    static seq_t *flat = NULL;
    static size_t flat_len = 0;   /* number of elements currently stored */
    static size_t flat_cap = 0;   /* allocated capacity of flat */
    static idx_t next_row = 1;    /* next k that has not yet been processed */

    /* Extend the table until it contains at least n elements. */
    while (flat_len < n)
    {
        /* Build the row for k = next_row. */
        uint64_t kval = (uint64_t)next_row;
        int bitlen = floor_log2_uint64(kval) + 1;  /* number of bits in k */

        /* Temporary buffer for distinct substrings (max 2145 for 64‑bit numbers). */
        seq_t row_vals[2300];
        int row_cnt = 0;

        /* Enumerate all binary substrings of the bit string of k. */
        for (int start = 0; start < bitlen; ++start)
        {
            seq_t cur = 0;
            for (int pos = start; pos < bitlen; ++pos)
            {
                /* Shift left and add next bit. */
                cur = (cur << 1) | ((kval >> (bitlen - 1 - pos)) & UINT64_C(1));
                if (cur == 0) continue;          /* skip the zero substring */

                /* Linear search for uniqueness. */
                int exists = 0;
                for (int j = 0; j < row_cnt; ++j)
                {
                    if (row_vals[j] == cur) { exists = 1; break; }
                }
                if (!exists) { row_vals[row_cnt++] = cur; }
            }
        }

        /* Sort the distinct values in increasing order. */
        for (int i = 1; i < row_cnt; ++i)
        {
            seq_t key = row_vals[i];
            int j = i - 1;
            while (j >= 0 && row_vals[j] > key)
            {
                row_vals[j + 1] = row_vals[j];
                --j;
            }
            row_vals[j + 1] = key;
        }

        /* Append the row to the flat array. */
        size_t new_len = flat_len + (size_t)row_cnt;
        grow_seq(&flat, &flat_cap, new_len);
        for (int i = 0; i < row_cnt; ++i)
            flat[flat_len + i] = row_vals[i];
        flat_len = new_len;

        ++next_row;
    }

    return flat[n - 1];
}
