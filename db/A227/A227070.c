/*
 * A227070  Powers n such that the set s(n) = {k > 0 such that k^n ends with
 *          k} does not occur for smaller n.
 * https://oeis.org/A227070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more, base
 * Formula:   a(1) = 1; for n>=2, a(n) = H(n-2)+1, where H(k) is the (k+1)-st
 *            number of the form 2^i·5^j in ascending order
 * Range:     u8 n <= 21, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <vector>
#include <cstddef>
#else
#include <stddef.h>
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    /* Handle the offset */
    if (n == 1)
        return (seq_t)1;

    /* Index of the required Hamming number (2^i·5^j) */
    size_t h_idx = (size_t)(n - 2);   /* zero‑based */

#if defined(__cplusplus)
    /* ------------------------------------------------------------
       C++ mode – use std::vector to store the sequence
       ------------------------------------------------------------ */
    static std::vector<seq_t> h_cache;
    static size_t idx2 = 0, idx5 = 0;

    /* Ensure the cache contains at least h_idx+1 elements */
    if (h_cache.size() <= h_idx) {
        if (h_cache.empty()) {
            h_cache.reserve(64);
            h_cache.push_back((seq_t)1);   /* first Hamming number */
            idx2 = idx5 = 0;
        }
        while (h_cache.size() <= h_idx) {
            seq_t cand2 = h_cache[idx2] * (seq_t)2;
            seq_t cand5 = h_cache[idx5] * (seq_t)5;
            seq_t next = (cand2 < cand5) ? cand2 : cand5;
            if (h_cache.back() != next)
                h_cache.push_back(next);
            if (next == cand2) ++idx2;
            if (next == cand5) ++idx5;
        }
    }
    return h_cache[h_idx] + (seq_t)1;
#else
    /* ------------------------------------------------------------
       C mode – use manual dynamic array
       ------------------------------------------------------------ */
    static seq_t *h_cache = NULL;
    static size_t h_len = 0, h_cap = 0;
    static size_t idx2 = 0, idx5 = 0;

    /* Ensure capacity and fill the cache up to the required index */
    if (h_len == 0) {
        /* Initial allocation and first element */
        h_cap = 64;
        h_cache = (seq_t *)malloc(h_cap * sizeof(seq_t));
        if (!h_cache) return (seq_t)0;   /* allocation failure */
        h_cache[0] = (seq_t)1;
        h_len = 1;
        idx2 = idx5 = 0;
    }

    if (h_len <= h_idx) {
        /* Grow the buffer if necessary */
        if (h_cap < h_idx + 1) {
            size_t newcap = h_cap * 2;
            if (newcap < h_idx + 1) newcap = h_idx + 1;
            seq_t *tmp = (seq_t *)realloc(h_cache, newcap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failure */
            h_cache = tmp;
            h_cap   = newcap;
        }

        while (h_len <= h_idx) {
            seq_t cand2 = h_cache[idx2] * (seq_t)2;
            seq_t cand5 = h_cache[idx5] * (seq_t)5;
            seq_t next  = (cand2 < cand5) ? cand2 : cand5;

            if (h_len == 0 || h_cache[h_len - 1] != next) {
                h_cache[h_len] = next;
                ++h_len;
            }
            if (next == cand2) ++idx2;
            if (next == cand5) ++idx5;
        }
    }

    return h_cache[h_idx] + (seq_t)1;
#endif
}
