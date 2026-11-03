/*
 * A044909  Numbers whose base-8 run lengths alternate: odd, even, odd, ...
 * https://oeis.org/A044909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base‑8 run lengths
 *            alternate odd, even, odd, ... (starting with an odd run)
 * Range:     u8 n <= 31, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Check whether x satisfies the alternating‑parity condition. */
static bool parity_ok(seq_t x)
{
    if (x == 0) return false;          /* not needed but safe */
    uint8_t digits[128];
    size_t len = 0;

    seq_t t = x;
    while (t > 0) {
        digits[len++] = (uint8_t)(t % (seq_t)8);
        t /= (seq_t)8;
    }

    /* No digits unlikely, but keep it safe */
    if (len == 0) return false;

    /* Process runs from the most significant digit downwards */
    uint8_t prev = digits[len - 1];
    size_t runlen = 1;
    bool odd_expected = true;          /* first run must be odd */

    for (size_t i = len - 1; i-- > 0; ) {
        uint8_t d = digits[i];
        if (d == prev) {
            ++runlen;
        } else {
            /* Check parity of the completed run */
            if ((odd_expected && (runlen & 1)) || (!odd_expected && !(runlen & 1))) {
                /* valid run, proceed */
            } else {
                return false;
            }
            odd_expected = !odd_expected;   /* toggle expectation */
            prev = d;
            runlen = 1;
        }
    }

    /* Final run check */
    if ((odd_expected && (runlen & 1)) || (!odd_expected && !(runlen & 1))) {
        return true;
    }
    return false;
}

/* Allocate and store terms progressively; keeps results for future
   calls regardless of order. */
static seq_t *s_cache = NULL;   /* array of computed terms   */
static idx_t  s_len   = 0;       /* number of terms stored   */
static idx_t  s_cap   = 0;       /* current cache capacity   */
static seq_t  s_next  = 1;       /* next candidate number    */

seq_t a(idx_t n)
{
    /* Extend cache until it contains at least n terms */
    while (s_len < n) {
        /* Grow storage if necessary */
        if (s_len == s_cap) {
            idx_t new_cap = s_cap ? s_cap << 1 : (idx_t)16;
            s_cache = mem_resize(seq_t, s_cache, new_cap);
            s_cap = new_cap;
        }

        if (parity_ok(s_next)) {
            s_cache[s_len++] = s_next;
        }
        ++s_next;
    }
    return s_cache[n - 1];
}
