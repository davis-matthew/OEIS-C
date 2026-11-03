/*
 * A044902  Base 16 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force enumeration: the nth positive integer whose
   hexadecimal representation has run lengths alternating even,
   odd, even, …, starting with an even run. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Return true if x has hexadecimal run lengths alternating
   even, odd, even, …, starting with an even run. */
static bool hex_runs_alternate(seq_t x)
{
    if (x == 0) {
        return false;            /* "0" has a single odd run */
    }

    /* Store hex digits least‑significant first. This array is
       large enough for 128‑bit numbers (32 hex digits). */
    uint8_t digits[128];
    size_t sz = 0;
    seq_t v = x;
    while (v != 0) {
        digits[sz++] = (uint8_t)(v % 16);
        v /= 16;
    }

    /* Examine runs from most‑significant to least‑significant. */
    int idx = (int)sz - 1;
    bool expect_even = true;     /* first run must be even */
    while (idx >= 0) {
        uint8_t cur = digits[idx];
        int runlen = 0;
        while (idx >= 0 && digits[idx] == cur) {
            runlen++;
            idx--;
        }
        if (expect_even) {
            if (runlen & 1) {        /* odd length when even expected */
                return false;
            }
        } else {
            if ((runlen & 1) == 0) { /* even length when odd expected */
                return false;
            }
        }
        expect_even = !expect_even;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = 0;
    while (count < n) {
        candidate++;                     /* next integer */
        if (hex_runs_alternate(candidate)) {
            count++;
        }
    }
    return candidate;
}
