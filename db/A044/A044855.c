/*
 * A044855  Positive integers having no fewer base-14 runs of even length than
 *          odd.
 * https://oeis.org/A044855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine returns the n‑th term of OEIS sequence A044855:
 * the smallest positive integer whose base‑14 representation contains
 * at least as many runs of even length as runs of odd length.
 * 
 * For each integer x the sequence of digits in base 14 is examined.
 * A «run» is a maximal block of equal consecutive digits.  For each
 * run its length is counted as even or odd; the integer is accepted
 * when the number of even runs is not smaller than the number of odd runs.
 * 
 * The implementation keeps a static cache of all terms computed so far,
 * extending it on demand.  The test for each candidate integer is done
 * in O(number of digits), which is negligible for the sizes required
 * (the 10 000th term is well below 2 × 10¹⁵, far within 64‑bit range).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool even_runs_ge_odd(uint64_t v) {
    uint8_t digits[64];
    size_t len = 0;
    if (v == 0) {
        digits[len++] = 0;
    } else {
        while (v != 0) {
            digits[len++] = (uint8_t)(v % 14);
            v /= 14;
        }
    }

    size_t i = len - 1;          /* most significant digit index */
    uint8_t curr = digits[i];
    size_t runlen = 1;
    int even_runs = 0, odd_runs = 0;

    while (i > 0) {
        if (digits[i - 1] == curr) {
            runlen++;
        } else {
            if (runlen % 2 == 0)
                even_runs++;
            else
                odd_runs++;
            curr = digits[i - 1];
            runlen = 1;
        }
        i--;
    }
    /* final run */
    if (runlen % 2 == 0)
        even_runs++;
    else
        odd_runs++;

    return even_runs >= odd_runs;
}

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t checked = 0;   /* last integer examined */

    /* extend cache until we have n terms */
    while (cache_len < n) {
        checked++;  /* candidate integer */
        if (even_runs_ge_odd(checked)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) exit(EXIT_FAILURE);  /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)checked;
        }
    }
    return cache[n - 1];
}
