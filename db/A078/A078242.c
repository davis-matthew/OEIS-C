/*
 * A078242  Smallest multiple of n using only digits 0 and 3.
 * https://oeis.org/A078242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest positive integer consisting only of the digits 0
 *            and 3 that is divisible by n. The program searches BFS over the
 *            remainders modulo n and reconstructs the smallest good number
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 107, u64 n <= 1187,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ==================== implementation ==================== */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence */
    if (n == 0) return 0;

    /* Allocate arrays to keep BFS state.
       The number of distinct remainders is at most n. */
    size_t N = (size_t)n;
    idx_t *parent = (idx_t *)malloc(N * sizeof(idx_t));
    char    *digit  = (char *)malloc(N * sizeof(char));
    bool    *visited = (bool *)malloc(N * sizeof(bool));
    idx_t *queue = (idx_t *)malloc(N * sizeof(idx_t));

    if (!parent || !digit || !visited || !queue) {
        free(parent); free(digit); free(visited); free(queue);
        return 0;
    }

    /* initialise */
    memset(visited, 0, N * sizeof(bool));
    idx_t start = 3 % n;
    visited[start] = true;
    parent[start] = (idx_t)-1;          /* sentinel for the root */
    digit[start]  = '3';
    size_t head = 0, tail = 0;
    queue[tail++] = start;

    /* immediate success if n divides 3 */
    if (start == 0) {
        seq_t result = (seq_t)3;
        free(parent); free(digit); free(visited); free(queue);
        return result;
    }

    /* BFS over the remainders */
    while (head < tail) {
        idx_t cur = queue[head++];
        for (int d = 0; d <= 3; d += 3) {
            idx_t nxt = (cur * 10 + d) % n;
            if (!visited[nxt]) {
                visited[nxt] = true;
                parent[nxt] = cur;
                digit[nxt]  = (char)('0' + d);

                if (nxt == 0) {
                    /* reconstruct the number by walking back the parent chain */
                    /* first collect digits in reverse order */
                    char revbuf[1024];
                    size_t revlen = 0;
                    idx_t r = nxt;
                    while (r != (idx_t)-1) {
                        revbuf[revlen++] = digit[r];
                        r = parent[r];
                    }
                    /* reverse to get proper order */
                    seq_t result = (seq_t)0;
                    for (size_t i = 0; i < revlen; ++i) {
                        result = result * (seq_t)10 + (seq_t)(revbuf[revlen - 1 - i] - '0');
                    }

                    free(parent); free(digit); free(visited); free(queue);
                    return result;
                }

                queue[tail++] = nxt;
            }
        }
    }

    /* Should never reach here for n >= 1 */
    free(parent); free(digit); free(visited); free(queue);
    return 0;
}
