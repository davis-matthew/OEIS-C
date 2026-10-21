/*
 * A074157  Smallest multiple of n which uses no digits of n, or 0 if no such
 *          number exists.
 * https://oeis.org/A074157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 623, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest multiple of n that contains none of the decimal digits of n,
   or 0 if no such number exists.  The algorithm performs a breadth‑first
   search on remainders modulo n over the set of allowed digits. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* n is guaranteed to be >= 1 by the OEIS definition */

    /* Determine which decimal digits are forbidden for this n. */
    bool forbidden[10] = {false};
    idx_t tmp = n;
    while (tmp > 0) {
        forbidden[tmp % 10] = true;
        tmp /= 10;
    }

    /* Build array of allowed digits (0–9 not forbidden). */
    uint8_t allowed[10];
    size_t allow_cnt = 0;
    for (size_t d = 0; d < 10; ++d)
        if (!forbidden[d]) {
            if (allow_cnt < 10) allowed[allow_cnt++] = (uint8_t)d;
        }

    /* If there are no non‑zero allowed digits, no positive number can be built. */
    bool has_nonzero = false;
    for (size_t i = 0; i < allow_cnt; ++i)
        if (allowed[i] != 0) { has_nonzero = true; break; }
    if (!has_nonzero) return (seq_t)0;

    /* Quick check for a one‑digit multiple. */
    for (size_t i = 0; i < allow_cnt; ++i) {
        uint8_t d = allowed[i];
        if (d == 0) continue;           /* leading digit cannot be zero */
        if ((idx_t)d % n == 0) return (seq_t)d;
    }

    /* BFS over remainders modulo n. */
    size_t m = (size_t)n;              /* m == n, the number of possible remainders */
    bool *visited = (bool*)malloc(m * sizeof(bool));
    idx_t *prev    = (idx_t*)malloc(m * sizeof(idx_t));
    uint8_t *pd    = (uint8_t*)malloc(m * sizeof(uint8_t));
    if (!visited || !prev || !pd) {
        free(visited); free(prev); free(pd);
        return (seq_t)0;
    }
    for (size_t i = 0; i < m; ++i) {
        visited[i] = false;
        prev[i]    = (idx_t)m;        /* sentinel value for uninitialized */
    }

    /* Queue for BFS */
    idx_t *queue = (idx_t*)malloc(m * sizeof(idx_t));
    if (!queue) {
        free(visited); free(prev); free(pd);
        return (seq_t)0;
    }
    size_t qh = 0, qt = 0;

    /* Enqueue initial states: first digits that are non‑zero. */
    for (size_t i = 0; i < allow_cnt; ++i) {
        uint8_t d = allowed[i];
        if (d == 0) continue;
        idx_t r = (idx_t)d % n;
        if (!visited[r]) {
            visited[r] = true;
            prev[r]    = (idx_t)m;    /* start sentinel */
            pd[r]      = d;
            if (r == 0) {
                free(visited); free(prev); free(pd); free(queue);
                return (seq_t)d;
            }
            queue[qt++] = r;
        }
    }

    /* BFS loop */
    while (qh < qt) {
        idx_t cur = queue[qh++];
        for (size_t i = 0; i < allow_cnt; ++i) {
            uint8_t d = allowed[i];
            idx_t nr = (cur * 10 + d) % n;
            if (!visited[nr]) {
                visited[nr] = true;
                prev[nr]    = cur;
                pd[nr]      = d;
                if (nr == 0) {
                    /* Reconstruct the number */
                    size_t len = 0;
                    uint8_t *digits_rev = (uint8_t*)malloc((m + 2) * sizeof(uint8_t));
                    if (!digits_rev) {
                        free(visited); free(prev); free(pd); free(queue);
                        return (seq_t)0;
                    }
                    idx_t r = nr;
                    while (prev[r] != (idx_t)m) {
                        digits_rev[len++] = pd[r];
                        r = prev[r];
                    }
                    digits_rev[len++] = pd[r];  /* first digit */

                    seq_t result = 0;
                    for (size_t j = 0; j < len; ++j)
                        result = result * 10 + (seq_t)digits_rev[len - 1 - j];

                    free(visited); free(prev); free(pd); free(queue);
                    free(digits_rev);
                    return result;
                }
                queue[qt++] = nr;
            }
        }
    }

    /* No solution found */
    free(visited); free(prev); free(pd); free(queue);
    return (seq_t)0;
}
