/*
 * A161772  Number of pattern sequences in bases 2 through 30 when the "sum of
 *          squares of digits" function is applied. In other words, A000216 is
 *          applied in other base systems, and the resulting number of closed
 *          patterns is counted.
 * https://oeis.org/A161772
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of non‑zero cycles of the “sum of squares of
 *            digits” function in base n. The function traverses the
 *            functional graph defined by f_b(x) = sum of squares of the
 *            base‑b digits of x and counts the distinct cycles that are
 *            reached from all integers ≥ 1
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   sumsq_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sumsq_base.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* b == n (base) */
    idx_t b = n;

    /* Compute the upper bound M for the search:
       M is the least integer such that M >= (b-1)^2 * (floor_log_b(M)+1). */
    idx_t maxsq = (b - 1) * (b - 1);   /* (b-1)^2 */
    idx_t M = maxsq;
    while (1) {
        /* Compute floor_log_b(M) + 1 = number of digits of M in base b */
        idx_t power = 1;
        idx_t digits = 0;
        while (power <= M) {
            power *= b;
            digits++;
        }
        idx_t newM = maxsq * digits;
        if (newM == M)
            break;
        M = newM;
    }

    /* Allocate visited state: 0 = unvisited, 1 = on current path, 2 = finished */
    uint8_t *state = (uint8_t *)calloc((size_t)(M + 1), sizeof(uint8_t));
    if (!state) return (seq_t)0;          /* fallback on allocation failure */

    seq_t cycles = 0;

    for (idx_t start = 1; start <= M; ++start) {
        if (state[start] != 0)
            continue;

        /* Follow the orbit from start until we reach a visited node */
        idx_t cur = start;
        while (state[cur] == 0) {
            state[cur] = 1;
            cur = (idx_t)sumsq_base((seq_t)cur, b);
        }

        /* If the node we hit is still on the current path, we've found a new cycle */
        if (state[cur] == 1)
            cycles++;

        /* Mark all nodes on this path as finished */
        cur = start;
        while (state[cur] == 1) {
            state[cur] = 2;
            cur = (idx_t)sumsq_base((seq_t)cur, b);
        }
    }

    free(state);
    return cycles;
}
