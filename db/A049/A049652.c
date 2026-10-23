/*
 * A049652  a(n) = (F(3*n+2) - 1)/4, where F=A000045 (the Fibonacci sequence).
 * https://oeis.org/A049652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = 4*a(n+1) + a(n) + 1, a(0) = 0, a(1) = 1 This recurrence
 *            is equivalent to a(n) = (F(3*n+2) - 1)/4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If seq_t is a multi‑precision type the built‑in arithmetic operators
   are overloaded so the same code works for 8‑, 16‑, 32‑, 64‑ or 128‑bit
   integers as well as arbitrary‑precision integers. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* resize cache if needed */
    if (n >= cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) {
            /* memory allocation failed – abort sequence generation */
            abort();
        }
        cache = tmp;
        /* initialise missing entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = 0;
            } else if (i == 1) {
                cache[i] = 1;
            } else {
                /* recurrence: a(i) = 4*a(i-1) + a(i-2) + 1 */
                seq_t t = cache[i - 1] * 4;   /* 4*a(i-1) */
                t = t + cache[i - 2];         /* + a(i-2) */
                t = t + 1;                    /* + 1   */
                cache[i] = t;
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}
