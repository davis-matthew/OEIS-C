/*
 * A115518  Numbers n such that the sum of the digits of n times the sum of
 *          the digits squared of n equals n.
 * https://oeis.org/A115518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   digit_sum(n) * digit_square_sum(n) == n
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: full, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   digit_sum, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Helper: sum of squares of decimal digits of a 64‑bit integer. */
static uint64_t digit_square_sum(uint64_t x)
{
    uint64_t sum = 0;
    while (x > 0) {
        uint64_t d = x % 10;
        sum += d * d;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    static seq_t *vals = NULL;
    static size_t vals_len = 0;
    static size_t vals_cap = 0;
    static bool init = false;

    if (!init) {
        init = true;
        /* According to the inequality 10^{d-1} <= 729*d^2,
           any solution must have at most 5 digits, i.e.
           n <= 9*5 * 81*5 = 18225. */
        const idx_t MAX = 18225;
        for (idx_t x = 1; x <= MAX; ++x) {
            seq_t s = digit_sum((seq_t)x);
            seq_t t = (seq_t)digit_square_sum((uint64_t)x);
            if (s * t == (seq_t)x) {
                append_candidate(&vals, &vals_len, &vals_cap, (seq_t)x);
            }
        }
    }

    if (n == 0 || n > vals_len) {
        /* Undefined for out‑of‑range indices; return 0. */
        return 0;
    }
    return vals[n - 1];
}
