/*
 * A050003  Numbers k such that A050000(k) > A050000(k+1).
 * https://oeis.org/A050003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with A050000(k) > A050000(k+1); computed by
 *            scanning A050000 (a(1)=1; a(n)=floor(a(n-1)/2) if new and
 *            nonzero, else 3*a(n-1))
 * Range:     u8 n <= 15, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t A050000_a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_cap = 0, seq_len = 0;

    static uint8_t *seen = NULL;
    static size_t seen_cap = 0;

    if (seq_len == 0) {
        seq_cap = 16;
        seq = mem_new(seq_t, seq_cap);
        seq[0] = (seq_t)1;
        seq_len = 1;

        seen_cap = 1;
        seen = (uint8_t *)malloc((seen_cap + 1) * sizeof(uint8_t));
        memset(seen, 0, seen_cap + 1);
        seen[1] = 1;
    }

    while (seq_len < n) {
        seq_t prev = seq[seq_len - 1];
        seq_t cand = prev / 2;

        bool halve_ok = false;
        if (cand != (seq_t)0) {
            if ((size_t)cand > seen_cap) {
                halve_ok = true;
            } else if (!seen[(size_t)cand]) {
                halve_ok = true;
            }
        }

        seq_t new_val;
        if (halve_ok)
            new_val = cand;
        else
            new_val = prev * (seq_t)3;

        if (seq_len >= seq_cap) {
            size_t new_cap = seq_cap * 2;
            seq = mem_resize(seq_t, seq, new_cap);
            seq_cap = new_cap;
        }
        seq[seq_len] = new_val;

        size_t new_max = (size_t)new_val;
        if (new_max > seen_cap) {
            size_t new_seen_cap = new_max;
            if (new_seen_cap < seen_cap * 2)
                new_seen_cap = seen_cap * 2;
            seen = (uint8_t *)realloc(seen, (new_seen_cap + 1) * sizeof(uint8_t));
            memset(seen + seen_cap + 1, 0, (new_seen_cap - seen_cap));
            seen_cap = new_seen_cap;
        }
        seen[(size_t)new_val] = 1;

        seq_len++;
    }

    return seq[n - 1];
}

seq_t a(idx_t n)
{
    static idx_t *found = NULL;
    static idx_t found_cap = 0, found_len = 0;
    static idx_t next_k = 1;

    while (found_len < n) {
        idx_t k = next_k++;
        if (A050000_a(k) > A050000_a(k + 1)) {
            if (found_len >= found_cap) {
                found_cap = found_cap ? found_cap * 2 : 64;
                found = (idx_t *)realloc(found, (size_t)found_cap * sizeof(idx_t));
            }
            found[found_len++] = k;
        }
    }
    return (seq_t)found[n - 1];
}
