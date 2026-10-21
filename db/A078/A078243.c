/*
 * A078243  Smallest multiple of n using only digits 0 and 4.
 * https://oeis.org/A078243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 26, u64 n <= 998, u128 n <= 998,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest multiple of n using only digits 0 and 4.
 * The number is obtained by a breadth‑first search over remainders
 * modulo n.  The BFS yields the shortest string (fewest digits)
 * that is a multiple of n, and the digits are 0 or 4 only. */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* special case n == 1 */
    if (n == 1) return (seq_t)4;

    /* start with the digit 4 */
    idx_t rem = 4 % n;
    if (rem == 0)               /* n divides 4, the answer is 4 */
        return (seq_t)4;

    size_t N = (size_t)n;
    idx_t *queue = (idx_t*)malloc(N * sizeof(idx_t));
    int32_t *parent = (int32_t*)malloc(N * sizeof(int32_t));
    unsigned char *digit = (unsigned char*)malloc(N * sizeof(unsigned char));
    unsigned char *visited = (unsigned char*)calloc(N, 1);

    if (!queue || !parent || !digit || !visited) {
        /* allocation failure – return 0 (undefined case) */
        free(queue); free(parent); free(digit); free(visited);
        return (seq_t)0;
    }

    size_t qhead = 0, qtail = 0;
    parent[rem] = -1;
    digit[rem] = 4;
    visited[rem] = 1;
    queue[qtail++] = rem;

    idx_t found = -1;
    while (qhead < qtail) {
        idx_t r = queue[qhead++];
        for (int d = 0; d <= 4; d += 4) {          /* digits 0 and 4 */
            idx_t nxt = (r * 10 + d) % n;
            if (!visited[nxt]) {
                visited[nxt] = 1;
                parent[nxt] = (int32_t)r;
                digit[nxt] = (unsigned char)d;
                queue[qtail++] = nxt;
                if (nxt == 0) {
                    found = nxt;
                    break;
                }
            }
        }
        if (found == 0) break;
    }

    /* reconstruct the number from remainder 0 */
    size_t len = 0;
    idx_t cur = found;
    char *buf = (char*)malloc((N + 10) * sizeof(char));
    if (!buf) {
        free(queue); free(parent); free(digit); free(visited);
        return (seq_t)0;
    }

    while (cur != -1) {
        buf[len++] = digit[cur] + '0';
        cur = parent[cur];
    }
    for (size_t i = 0; i < len / 2; ++i) {
        char t = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = t;
    }
    buf[len] = '\0';

    /* convert decimal string to seq_t */
    seq_t result = (seq_t)0;
    seq_t ten = (seq_t)10;
    for (size_t i = 0; i < len; ++i) {
        result = result * ten + (seq_t)(buf[i] - '0');
    }

    free(buf);
    free(queue);
    free(parent);
    free(digit);
    free(visited);
    return result;
}
