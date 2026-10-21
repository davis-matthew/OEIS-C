/*
 * A067044  Smallest positive k such that k*n contains only even digits.
 * https://oeis.org/A067044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the smallest positive k such that k·n consists only of
 *            even decimal digits. This implementation performs a
 *            breadth‑first search on the remainders modulo n using only the
 *            allowed digits {0,2,4,6,8}. When a remainder 0 is reached, the
 *            corresponding even‑digit multiple M has been found; k is...
 * Range:     u8 n <= 8, u16 n <= 997, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, memset */
#include <stdbool.h>  /* bool type */
#include <string.h>   /* memset */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety, although n≥1 in the OEIS sequence */

    /* Allocate data structures for BFS */
    bool  *visited = (bool *)malloc(sizeof(bool) * n);
    int   *parent  = (int  *)malloc(sizeof(int)  * n);
    int   *digit   = (int  *)malloc(sizeof(int)  * n);
    int   *queue   = (int  *)malloc(sizeof(int)  * n);

    if (!visited || !parent || !digit || !queue) {
        /* In case of allocation failure, return 0 as a safe fallback. */
        free(visited); free(parent); free(digit); free(queue);
        return 0;
    }
    memset(visited, 0, sizeof(bool) * n);

    /* First digits cannot be 0: 2,4,6,8 */
    int qh = 0, qt = 0;
    for (int d = 2; d <= 8; d += 2) {
        int r = d % (int)n;
        if (!visited[r]) {
            visited[r] = true;
            parent[r]  = -1;
            digit[r]   = d;
            queue[qt++] = r;
            if (r == 0) goto found;   /* immediate solution */
        }
    }

    /* BFS over remainders */
    while (qh < qt) {
        int r = queue[qh++];
        for (int d = 0; d <= 8; d += 2) {
            int r2 = (int)(((long long)r * 10 + d) % (long long)n);
            if (!visited[r2]) {
                visited[r2] = true;
                parent[r2]  = r;
                digit[r2]   = d;
                queue[qt++] = r2;
                if (r2 == 0) goto found;
            }
        }
    }

found:
    /* Reconstruct the minimal multiple M as a decimal string */
    char buf[65536];   /* large enough for all practical n up to 10000 */
    int  pos = 0;
    int  cur = 0;      /* remainder 0 */

    /* Backtrack from remainder 0 to the start */
    while (parent[cur] != -1) {
        buf[pos++] = '0' + digit[cur];
        cur = parent[cur];
    }
    buf[pos++] = '0' + digit[cur];   /* first digit */

    /* Reverse to obtain the correct order */
    for (int i = 0; i < pos / 2; ++i) {
        char t = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = t;
    }

    /* Convert the string M to seq_t and compute k = M / n */
    seq_t M = 0;
    for (int i = 0; i < pos; ++i) {
        M = M * 10 + (seq_t)(buf[i] - '0');
    }
    seq_t k = M / n;   /* exact division, as M is guaranteed to be a multiple of n */

    /* Clean up */
    free(visited);
    free(parent);
    free(digit);
    free(queue);

    return k;
}
