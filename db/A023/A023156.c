/*
 * A023156  Number of cycles of function f(x) = x^5 mod n.
 * https://oeis.org/A023156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 388, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of cycles of the function f(x) = x^5 mod n over {0,…,n‑1} */

/* Compute x^5 mod n using the seq_t arithmetic, then cast back to idx_t for array indices. */
static inline idx_t pow5_mod(idx_t x, idx_t mod)
{
    seq_t m = (seq_t) mod;
    seq_t y = (seq_t) x;
    seq_t y2 = (y * y) % m;      /* x^2 mod m */
    seq_t y4 = (y2 * y2) % m;     /* x^4 mod m */
    seq_t y5 = (y4 * y) % m;      /* x^5 mod m */
    return (idx_t) y5;
}

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For n == 0 the result is undefined by the OEIS specification, but we
       return 0 to avoid a crash when such a call occurs.           */
    if (n == 0) return (seq_t)0;

    /* visited[i] == 0  -> untouched
       visited[i] == marker (non‑zero) -> visited in the current DFS chain   */
    idx_t *visited = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!visited) return (seq_t)0;      /* allocation failure: safe fallback */

    /* initialise visited */
    for (idx_t i = 0; i < n; ++i) visited[i] = 0;

    seq_t cycles = 0;

    for (idx_t start = 0; start < n; ++start)
    {
        if (visited[start] != 0) continue;   /* already processed */

        idx_t marker = start + 1;                /* unique non‑zero marker */
        idx_t cur = start;

        /* follow the functional graph until a node repeats */
        while (visited[cur] == 0)
        {
            visited[cur] = marker;
            cur = pow5_mod(cur, n);
        }

        /* if the repeated node was in this exploration, we found a new cycle */
        if (visited[cur] == marker) cycles++;
    }

    free(visited);
    return cycles;
}
