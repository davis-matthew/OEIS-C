/*
 * A185867  Numbers m whose decimal digits are a subsequence of the decimal
 *          digits of k*m for some 1 < k < 10.
 * https://oeis.org/A185867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A185867 – numbers m whose decimal digits are a subsequence
 * of the decimal digits of k*m for some 1 < k < 10.
 * Computed by brute‑force search.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_digits.h"

/* Test whether `sub` (length `sub_len`) is a subsequence of `sup`
 * (length `sup_len`).  Both arrays contain most‑significant‑first digits. */
static inline bool is_subsequence(const uint8_t *sub, size_t sub_len,
                                  const uint8_t *sup, size_t sup_len)
{
    size_t i = 0, j = 0;
    while (i < sub_len && j < sup_len) {
        if (sub[i] == sup[j]) {
            ++i; ++j;
        } else {
            ++j;
        }
    }
    return i == sub_len;
}

/* Return the n-th term of sequence A185867. */
seq_t a(idx_t n)
{
    /* Buffers large enough for all intermediate values that fit in seq_t. */
    uint8_t buf_m[4096];
    uint8_t buf_prod[4096];
    size_t len_m, len_prod;

    idx_t count   = 0;          /* how many terms found so far            */
    idx_t m_candidate = 0;      /* start below the first term (2)         */

    while (count < n) {
        ++m_candidate;          /* candidate m                          */
        bool good = false;

        /* digits of m_candidate (computed once per candidate) */
        get_digits((seq_t)m_candidate, buf_m, &len_m);

        for (int k = 2; k <= 9 && !good; ++k) {
            seq_t prod = (seq_t)m_candidate * (seq_t)k;

            /* product must be at least as long as m_candidate */
            if (prod == 0) continue;
            get_digits(prod, buf_prod, &len_prod);
            if (len_prod >= len_m && is_subsequence(buf_m, len_m, buf_prod, len_prod)) {
                good = true;
            }
        }

        if (good) {
            ++count;
        }
    }

    return (seq_t)m_candidate;
}
