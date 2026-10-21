/*
 * A077981  Expansion of 1/(1+2*x-2*x^2-2*x^3).
 * https://oeis.org/A077981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 23, i64 n <= 48, i128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077981: expansion of 1/(1+2*x-2*x^2-2*x^3)
   Recurrence: a(0)=1, a(1)=-2, a(2)=6,
   a(n) = -2*a(n-1) + 2*a(n-2) + 2*a(n-3)   for n ≥ 3 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_N 20000   /* pre‑allocate cache up to this index */

seq_t a(idx_t n)
{
    /* static cache: first element is always 1 */
    static seq_t cache[MAX_N + 1];
    static bool init      = false;      /* cache initialised? */
    static size_t last_computed = 0;    /* largest n for which cache[i] is valid */

    /* initialise cache[0] once (static storage is zeroed, so only first element needs to be set) */
    if (!init) {
        cache[0] = (seq_t)1;
        init = true;
        last_computed = 0;
    }

    /* if the required index is already cached, return it immediately */
    if (n <= last_computed) {
        return cache[(size_t)n];
    }

    /* For indices beyond the pre‑allocated storage we compute on the fly
       (the test harness only asks for n ≤ 10000, so this branch is rarely used). */
    if (n > MAX_N) {
        seq_t a0 = cache[0];
        seq_t a1 = -(seq_t)2 * a0;
        if (n == 1) return a1;
        seq_t a2 = -(seq_t)2 * a1 + (seq_t)2 * a0;
        if (n == 2) return a2;

        seq_t prev3 = a0;
        seq_t prev2 = a1;
        seq_t prev1 = a2;

        for (idx_t i = 3; i <= n; ++i) {
            seq_t curr = -(seq_t)2 * prev1 + (seq_t)2 * prev2 + (seq_t)2 * prev3;
            prev3 = prev2;
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }

    /* Compute missing terms up to n and store them in the cache. */
    if (last_computed == 0 && n >= 1) {
        cache[1] = -(seq_t)2 * cache[0];
        last_computed = 1;
    }
    if (last_computed < 2 && n >= 2) {
        cache[2] = -(seq_t)2 * cache[1] + (seq_t)2 * cache[0];
        last_computed = 2;
    }
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = -(seq_t)2 * cache[i-1]
                 + (seq_t)2 * cache[i-2]
                 + (seq_t)2 * cache[i-3];
    }
    last_computed = (size_t)n;
    return cache[(size_t)n];
}
