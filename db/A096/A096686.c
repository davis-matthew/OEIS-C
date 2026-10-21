/*
 * A096686  Least k such that decimal representation of k*n contains only
 *          digits 0 and 7.
 * https://oeis.org/A096686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = least k such that decimal representation of k*n *
 *            consists only of the digits 0 and 7. * Computed via
 *            breadth‑first search over the remainders modulo n
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 17, u64 n <= 998,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/* ------------------------------------------------------------------
 *  Find the smallest positive integer k for which k*n is a decimal
 *  string consisting solely of the digits 0 and 7.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* n must be positive.  The function is never called with n == 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Special case: for n == 1 the minimal number is 7 itself. */
    if (n == 1) {
        return (seq_t)7;
    }

    /* Number of possible remainders modulo n. */
    idx_t N = n;

    /* Allocate state arrays.  All structures are indexed by
     * remainders modulo n (0 … n-1). */
    bool  *visited = (bool  *)malloc((size_t)N * sizeof(bool));
    idx_t *parent  = (idx_t *)malloc((size_t)N * sizeof(idx_t));
    char  *digit   = (char  *)malloc((size_t)N * sizeof(char));
    idx_t *queue   = (idx_t *)malloc((size_t)N * sizeof(idx_t));
    if (!visited || !parent || !digit || !queue) {
        free(visited); free(parent); free(digit); free(queue);
        return (seq_t)0; /* fallback on allocation failure */
    }

    /* Initialise the graph. */
    for (size_t i = 0; i < (size_t)N; ++i) {
        visited[i] = false;
        parent[i]  = (idx_t)-1;
    }

    /* Start BFS from the number "7". */
    idx_t start = 7 % n;
    visited[start] = true;
    parent[start] = N;           /* sentinel marking the root */
    digit[start]  = '7';

    idx_t head = 0, tail = 0;
    queue[tail++] = start;

    bool found = (start == 0);

    /* BFS over the remainder graph. */
    while (head < tail && !found) {
        idx_t r = queue[head++];
        for (int d = 0; d < 2; ++d) {          /* try digit 0 then 7 */
            int dig = (d == 0) ? 0 : 7;
            idx_t nr = (r * 10 + dig) % n;
            if (!visited[nr]) {
                visited[nr] = true;
                parent[nr]  = r;
                digit[nr]   = (char)('0' + dig);
                queue[tail++] = nr;
                if (nr == 0) {                 /* divisible by n */
                    found = true;
                    break;
                }
            }
        }
    }

    /* Reconstruct the minimal number m consisting of digits 0 and 7. */
    seq_t m = (seq_t)0;
    if (found) {
        /* Collect digits in reverse order (from last to first). */
        char *rev_digits = (char *)malloc((size_t)N * sizeof(char));
        if (!rev_digits) { /* allocation failure */
            free(visited); free(parent); free(digit); free(queue);
            return (seq_t)0;
        }

        idx_t r = 0;
        size_t rev_len = 0;
        while (parent[r] != N) {
            rev_digits[rev_len++] = digit[r];
            r = parent[r];
        }
        rev_digits[rev_len++] = digit[r];  /* the first (most significant) digit */

        /* Build m from the collected digits. */
        for (size_t i = rev_len; i-- > 0; ) {
            m = m * (seq_t)10 + (seq_t)(rev_digits[i] - '0');
        }

        free(rev_digits);
    } else {
        /* This situation should not occur for n >= 1. */
        free(visited); free(parent); free(digit); free(queue);
        return (seq_t)0;
    }

    seq_t k = m / (seq_t)n;  /* m is a multiple of n by construction */

    free(visited); free(parent); free(digit); free(queue);
    return k;
}
