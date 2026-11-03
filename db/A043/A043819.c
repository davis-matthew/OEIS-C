/*
 * A043819  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 4 mod 10.
 * https://oeis.org/A043819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: numbers whose base‑3 representation has a run count ≡ 4 (mod 10) */

#include <stdlib.h>
#include <stdbool.h>

/* Returns true iff the base‑3 representation of x has a run count
   congruent to 4 modulo 10. */
static bool trait_ok(seq_t x) {
    if (x == 0) return false;          /* 0 is not in the sequence */

    seq_t tmp = x;
    int runs = 1;                      /* first run */
    int prev = (int)(tmp % 3);
    tmp = tmp / 3;

    while (tmp > 0) {
        int d = (int)(tmp % 3);
        if (d != prev) {
            ++runs;
            prev = d;
        }
        tmp = tmp / 3;
    }
    return (runs % 10) == 4;
}

/* Static cache for the sequence values.
   200 000 is ample for n up to about 10 000. */
static seq_t cache[200000];
static size_t cache_len = 0;
static seq_t last_val = 0;   /* greatest number examined so far */

/* Return the nth term of sequence A043819 (OEIS offset 1). */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined for n == 0 */

    /* Generate terms until we have at least n of them. */
    if (cache_len < (size_t)n) {
        seq_t current = last_val + 1;

        while (cache_len < (size_t)n) {
            if (trait_ok(current)) {
                cache[cache_len++] = current;
            }
            last_val = current;
            ++current;
        }
    }
    return cache[(size_t)n - 1];
}
