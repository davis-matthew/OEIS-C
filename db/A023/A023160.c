/*
 * A023160  Number of cycles of function f(x) = x^9 mod n.
 * https://oeis.org/A023160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of cycles of f(x)=x^9 mod n in the functional digraph of
 *            f
 * Range:     u8 n <= 508, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9643 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of cycles of the map x → x^9 mod n */
#include <stdint.h>
#include <stdlib.h>

/* helper: fast exponentiation of x^9 mod n (fixed exponent 9) */
static inline seq_t pow9_mod(idx_t x, idx_t n) {
    seq_t a = (seq_t)x % (seq_t)n;   /* x mod n, keep in seq_t to avoid overflow   */
    seq_t a2 = (a * a) % (seq_t)n;   /* x^2  mod n */
    seq_t a4 = (a2 * a2) % (seq_t)n; /* x^4  mod n */
    seq_t a8 = (a4 * a4) % (seq_t)n; /* x^8  mod n */
    return (a * a8) % (seq_t)n;      /* x^9 = x * x^8  mod n */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                        /* offset starts at 1, but guard against 0 */
    size_t N = (size_t)n;                         /* number of elements 0..n-1               */

    /* state array: 0=unvisited, 1=visiting, 2=finished */
    uint8_t *state = (uint8_t*)malloc(N);
    if (!state) return 0;
    for (size_t i = 0; i < N; ++i) state[i] = 0;

    /* temporary stack for current DFS path */
    size_t *stack = (size_t*)malloc(N * sizeof(size_t));
    if (!stack) { free(state); return 0; }

    seq_t cycles = 0;

    for (size_t start = 0; start < N; ++start) {
        if (state[start] != 0) continue;         /* already processed               */

        size_t top = 0;
        size_t v = start;
        /* follow the orbit until we hit an already visited vertex */
        while (state[v] == 0) {
            state[v] = 1;
            stack[top++] = v;
            v = (size_t)pow9_mod((idx_t)v, n);
        }
        /* If we re-entered a vertex currently on the path, we found a cycle */
        if (state[v] == 1) cycles++;

        /* Mark all vertices in the current path as finished */
        while (top > 0) state[stack[--top]] = 2;
    }

    free(state);
    free(stack);
    return cycles;
}
