/*
 * A162311  Numbers such that A010060(n) = A010060(n+7).
 * https://oeis.org/A162311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = numbers such that the parity of the binary popcount of n
 *            equals the parity of the binary popcount of n+7
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9675 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Parity of the number of 1 bits in x: true if odd, false if even */
static inline bool parity_bitcount(seq_t x)
{
    bool p = false;
    while (x) {
        p ^= (bool)(x & 1);
        x >>= 1;
    }
    return p;
}

/* Cached sequence data */
static seq_t *sarr = NULL;
static size_t   scap = 0;
static size_t   slen = 0;
static idx_t    last_checked = 0;

/* Ensure that the cache contains at least n terms */
static void ensure_terms(idx_t n)
{
    if (slen >= n) return;

    if (!sarr) {
        scap = 16;
        sarr = mem_new(seq_t, scap);
    }

    while (slen < n) {
        /* Find the next candidate number */
        idx_t cand_idx = ++last_checked;
        seq_t cand = (seq_t)cand_idx;
        seq_t next = cand + (seq_t)7;

        if (parity_bitcount(cand) == parity_bitcount(next)) {
            if (slen == scap) {
                scap *= 2;
                sarr = mem_resize(seq_t, sarr, scap);
            }
            sarr[slen++] = cand;
        }
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return sarr[n - 1];
}
