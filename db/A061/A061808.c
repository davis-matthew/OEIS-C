/*
 * A061808  a(n) is the smallest number with all digits odd that is divisible
 *          by 2n-1.
 * https://oeis.org/A061808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = smallest number with all digits odd that is divisible by
 *            (2n-1)
 * Range:     u8 n <= 10, u16 n <= 300, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a breadth‑first search on remainders modulo (2n-1) */
/* to find the minimal odd‑digit multiple.  It works for any n ≥ 1. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* offset is 1 */
    idx_t m = 2*n - 1;                        /* modulus */
    if (m == 1) return 1;                     /* 1 divides everything */

    /* allocate data structures */
    size_t ms = (size_t)m;
    bool *visited    = (bool*)   malloc(ms * sizeof(bool));
    idx_t *parent    = (idx_t*)  malloc(ms * sizeof(idx_t));
    int  *used_digit = (int*)    malloc(ms * sizeof(int));
    idx_t *queue     = (idx_t*)  malloc(ms * sizeof(idx_t));

    if (!visited || !parent || !used_digit || !queue) {
        /* memory allocation failed – return 0 (should not happen in tests) */
        free(visited); free(parent); free(used_digit); free(queue);
        return 0;
    }

    /* initialise */
    for (size_t i = 0; i < ms; ++i) {
        visited[i]   = false;
        parent[i]    = m;               /* sentinel for root nodes */
    }

    const int DIGITS[5] = {1, 3, 5, 7, 9};

    /* BFS queue pointers */
    size_t head = 0, tail = 0;

    /* start with one‑digit numbers */
    for (int d = 0; d < 5; ++d) {
        idx_t r = (idx_t)DIGITS[d] % m;
        if (!visited[r]) {
            visited[r]   = true;
            /* parent already sentinel */
            used_digit[r] = DIGITS[d];
            queue[tail++] = r;
            if (r == 0) {                     /* found immediately */
                free(visited); free(parent);
                free(used_digit); free(queue);
                return DIGITS[d];
            }
        }
    }

    /* BFS */
    idx_t target = 0;   /* remainder 0 will be found */
    while (head < tail) {
        idx_t cur = queue[head++];
        for (int d = 0; d < 5; ++d) {
            idx_t nr = (cur * 10 + (idx_t)DIGITS[d]) % m;
            if (!visited[nr]) {
                visited[nr]   = true;
                parent[nr]    = cur;
                used_digit[nr] = DIGITS[d];
                queue[tail++] = nr;
                if (nr == 0) {                 /* solution found */
                    target = nr;
                    goto found;
                }
            }
        }
    }

found:
    /* reconstruct digits in reverse order */
    int tmp[1024];   /* should be large enough for all reachable lengths */
    int len = 0;
    idx_t r = target;
    while (true) {
        tmp[len++] = used_digit[r];
        if (parent[r] == m) break;            /* reached a root digit */
        r = parent[r];
    }

    /* build the result value */
    seq_t val = 0;
    for (int i = len - 1; i >= 0; --i) {
        val = val * 10 + (seq_t)tmp[i];
    }

    /* clean up */
    free(visited); free(parent);
    free(used_digit); free(queue);

    return val;
}
