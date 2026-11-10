/*
 * A061089  Duplicate of A067499.
 * https://oeis.org/A061089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n-th (from 0) power of 2 whose digit sum is also a power
 *            of 2; found by searching successive powers of 2
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 4, i64 n <= 5, i128 n <= 6,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 6 (2026-11-07)
 * Helpers:   digit_sum, is_pow2, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/digit_sum.h"
#include "../utils/is_pow2.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static bool   initialized   = false;
    static seq_t *terms         = NULL;
    static size_t terms_len     = 0;
    static size_t terms_cap     = 0;
    static seq_t  current_power = 0;
    static bool   exhausted     = false;

    if (!initialized) {
        initialized = true;
        terms_cap = 8;
        terms = mem_new(seq_t, terms_cap);
        if (!terms) return 0;
        current_power = 1;
        terms[0] = current_power;
        terms_len = 1;
    }

    while ((idx_t)terms_len <= n) {
        if (exhausted) return 0;
        seq_t next = current_power * 2;
        if (next <= current_power) {
            exhausted = true;
            return 0;
        }
        current_power = next;

        seq_t ds = digit_sum(current_power);
        if (is_pow2(ds)) {
            if (terms_len + 1 > terms_cap) {
                size_t ncap = terms_cap * 2;
                seq_t *nt = mem_resize(seq_t, terms, ncap);
                if (!nt) return 0;
                terms = nt;
                terms_cap = ncap;
            }
            terms[terms_len++] = current_power;
        }
    }

    return terms[(size_t)n];
}
