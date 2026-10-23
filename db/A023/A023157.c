/*
 * A023157  Number of cycles of function f(x) = x^6 mod n.
 * https://oeis.org/A023157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of cycles of the function f(x) = x^6 mod n
 * Range:     u8 n <= 2585, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow6_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow6_mod.h"

/* Return the number of cycles of x ↦ x^6 (mod n), for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n < 1, harmless */

    /* Static visitation state array: 0 = unvisited, 1 = in current DFS,
       2 = processed.  Reuse across calls for efficiency. */
    static uint8_t *state = NULL;
    static size_t state_cap = 0;

    size_t sz = (size_t)n;
    if (state_cap < sz) {
        uint8_t *tmp = (uint8_t *)realloc(state, sz);
        if (!tmp) return (seq_t)0;            /* allocation failure → 0 */
        state = tmp;
        state_cap = sz;
    }

    /* clear for new n */
    for (size_t i = 0; i < sz; ++i) state[i] = 0;

    seq_t cycles = 0;

    for (idx_t i = 0; i < n; ++i) {
        if (state[i] != 0) continue;          /* already processed */

        idx_t cur = i;
        /* walk until a visited node is met */
        while (state[cur] == 0) {
            state[cur] = 1;                    /* mark in current traversal */
            cur = pow6_mod(cur, n);
        }
        if (state[cur] == 1) {                /* closed a new cycle */
            ++cycles;
        }
        /* mark the whole visited path as processed */
        cur = i;
        while (state[cur] == 1) {
            state[cur] = 2;
            cur = pow6_mod(cur, n);
        }
    }
    return cycles;
}
