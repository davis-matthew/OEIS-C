/*
 * A066547  Let N = 123456789101112131415161718..., the concatenation of the
 *          natural numbers. a(n) is the n-digit number formed from the digits
 *          of N starting from the {n(n-1)/2 +1}th digit. Omit any leading
 *          zeros.
 * https://oeis.org/A066547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   dlen_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑digit block of the infinite concatenation of the natural
 * numbers, starting at digit position n*(n-1)/2 + 1.
 * Leading zeros are omitted.  The function works for any n ≥ 1.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/dlen_uint64.h"

/* Prefix array holding the cumulative number of digits in 1..i.  */
static uint64_t *pref      = NULL; /* pref[i] = total digits 1..i */
static size_t   pref_cap   = 0;     /* current allocation size    */
static size_t   pref_len   = 0;     /* highest i that has been set */

/* Ensure pref[] contains the cumulative digit counts at least up to
 * position ‘need’.  pref[pref_len] will be ≥ need after the call.  */
static void ensure_pref(uint64_t need) {
    if (!pref) {
        pref_cap = 1024;
        pref = (uint64_t *)calloc(pref_cap, sizeof(uint64_t));
        if (!pref) abort();
        pref[0] = 0;
        pref_len = 0;
    }
    while (pref[pref_len] < need) {
        uint64_t next = pref_len + 1;
        if (pref_len + 1 >= pref_cap) {
            size_t new_cap = pref_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(pref, new_cap * sizeof(uint64_t));
            if (!tmp) abort();
            pref = tmp;
            pref_cap = new_cap;
        }
        uint64_t digits = dlen_uint64(next);
        pref[next] = pref[pref_len] + digits;
        ++pref_len;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* off‑by‑zero case – the official offset is 1 */

    /* Position in the concatenated sequence where the block starts
     * (1‑based).  Use seq_t arithmetic for the triangular number. */
    seq_t nn = (seq_t)n;
    seq_t pos = nn * (nn - 1) / 2 + 1;

    /* Make sure the prefix array covers this position. */
    ensure_pref((uint64_t)pos);

    /* Binary search for the first number that contains the start
     * position.  pref[i] is the position after number i.  */
    idx_t lo = 1, hi = (idx_t)pref_len;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (pref[mid] >= (uint64_t)pos)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t m = lo;  /* m is the first number that contributes to the block */

    /* Offset of the start within the decimal representation of m. */
    uint64_t offset_in_num = (uint64_t)pos - pref[m - 1] - 1;

    seq_t result = 0;
    bool   started = false;
    idx_t  taken   = 0;

    for (idx_t cur = m; taken < n; ++cur) {
        /* Convert cur to decimal string.  The buffer is large enough
           for 64‑bit numbers.  */
        char buf[32];
        int  len = sprintf(buf, "%llu", (unsigned long long)cur);

        /* For the first number, start after the offset. */
        uint64_t start_idx = 0;
        if (cur == m) start_idx = offset_in_num;

        for (uint64_t i = start_idx; i < (uint64_t)len && taken < n; ++i) {
            int dig = buf[i] - '0';
            if (!started) {
                if (dig != 0) {
                    started = true;
                    result = (seq_t)dig;
                }
            } else {
                result = result * (seq_t)10 + (seq_t)dig;
            }
            ++taken;
        }
    }
    return result;  /* if all digits were zero, result stays 0 (impossible) */
}
