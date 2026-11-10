/*
 * A086451  Numbers not in A086335.
 * https://oeis.org/A086451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer not in A086335 (computed by
 *            generating A086335 terms with Hofstadter-type recurrence and
 *            sieving out the values present)
 * Range:     u8 n <= 11, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A086335_cache = NULL;
static size_t A086335_cap = 0;
static size_t A086335_len = 0;

static seq_t A086335_a(idx_t n)
{
    if (n <= 2) return 1;
    if ((size_t)n + 1 > A086335_cap) {
        size_t new_cap = (A086335_cap == 0) ? 8 : A086335_cap << 1;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, A086335_cache, new_cap);
        if (!tmp) return 0;
        A086335_cache = tmp;
        A086335_cap = new_cap;
    }
    if (A086335_len < 2) {
        A086335_cache[0] = 0;
        A086335_cache[1] = 1;
        A086335_cache[2] = 1;
        A086335_len = 2;
    }
    if ((size_t)n > A086335_len) {
        for (size_t ii = A086335_len + 1; ii <= (size_t)n; ++ii) {
            if (ii & 1) {
                size_t ap = (size_t)A086335_cache[ii - 1];
                A086335_cache[ii] = A086335_cache[ap] + A086335_cache[ii - ap];
            } else {
                size_t v1 = (size_t)A086335_cache[ii - 1];
                size_t v2 = (size_t)A086335_cache[ii - 2];
                A086335_cache[ii] = A086335_cache[ii - v1] + A086335_cache[ii - v2];
            }
        }
        A086335_len = (size_t)n;
    }
    return A086335_cache[(size_t)n];
}

seq_t a(idx_t n)
{
    size_t M = (size_t)n * 16 + 64;
    for (;;) {
        size_t L = M * 4 + 64;
        unsigned char *seen = (unsigned char *)calloc(M + 1, 1);
        if (!seen) return 0;
        A086335_a((idx_t)L);
        for (size_t i = 1; i <= L; ++i) {
            seq_t v = A086335_cache[i];
            if (v >= 1 && v <= (seq_t)M) seen[(size_t)v] = 1;
        }
        idx_t count = 0;
        for (size_t v = 1; v <= M; ++v) {
            if (!seen[v]) {
                ++count;
                if (count == n) {
                    free(seen);
                    return (seq_t)v;
                }
            }
        }
        free(seen);
        M *= 2;
    }
}
