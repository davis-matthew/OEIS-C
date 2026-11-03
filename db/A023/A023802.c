/*
 * A023802  Xenodromes: all digits in base 7 are different.
 * https://oeis.org/A023802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   a(n) = the n‑th nonnegative integer whose digits in base 7 are
 *            all * distinct (xenodromes)
 * Range:     u8 n <= 171, u16 n <= 5193, u32 n <= 11743, u64 n <= 11743,
 *            u128 n <= 11743, big any n
 * Validated: full, pass, 11743 terms through n = 11743 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A023802: xenodromes in base 7, the numbers whose base-7 digits are all
 * different (0 included); there are 11743 of them, all below 7^7. They are
 * listed once by a scan to 7^7 and cached (the original rescanned from 0 on
 * every call). */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *terms = NULL;
static size_t term_len = 0;

seq_t a(idx_t n)
{
    if (!terms) {
        terms = (uint32_t *)malloc(11743 * sizeof *terms);
        if (!terms) abort();
        for (uint32_t v = 0; v < 823543; ++v) {
            unsigned mask = 0, ok = 1;
            for (uint32_t x = v; x && ok; x /= 7) {
                unsigned d = 1u << (x % 7);
                ok = !(mask & d);
                mask |= d;
            }
            if (ok && term_len < 11743) terms[term_len++] = v;
        }
    }
    if (n < 1 || (size_t)n > term_len) return (seq_t)0;
    return (seq_t)terms[n - 1];
}
