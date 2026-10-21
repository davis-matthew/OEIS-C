/*
 * A096688  Least k such that decimal representation of k*n contains only
 *          digits 0 and 9.
 * https://oeis.org/A096688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the least k such that the decimal representation of k*n
 *            contains only digits 0 and 9. This implementation finds the
 *            smallest such 0‑9 multiple of n by breadth‑first search, then
 *            returns that multiple divided by n
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Undefined for n = 0 – return 0. */
    if (n == 0)
        return (seq_t)0;

    /* If n divides 9, the minimal number is 9 itself. */
    if (9 % n == 0)
        return (seq_t)(9 / n);

    /* Allocate BFS arrays. */
    idx_t *queue   = (idx_t*)malloc((size_t)n * sizeof(idx_t));
    bool  *visited = (bool*)malloc((size_t)n * sizeof(bool));
    idx_t *parent  = (idx_t*)malloc((size_t)n * sizeof(idx_t));
    unsigned char *digit = (unsigned char*)malloc((size_t)n * sizeof(unsigned char));
    if (!queue || !visited || !parent || !digit) {
        free(queue); free(visited); free(parent); free(digit);
        return (seq_t)0;
    }

    /* Initialization. */
    for (idx_t i = 0; i < n; ++i) visited[i] = false;
    const idx_t ROOT = n;   /* sentinel value for the start node. */
    idx_t head = 0, tail = 0;
    idx_t rem  = (idx_t)(9 % n);
    queue[tail++] = rem;
    visited[rem] = true;
    parent[rem] = ROOT;
    digit[rem] = '9';

    /* Breadth‑first search over remainders. */
    while (head < tail) {
        idx_t cur = queue[head++];
        for (int d = 0; d < 2; ++d) {          /* try appending 0 and 9 */
            unsigned char ch = (d == 0 ? '0' : '9');
            idx_t new_rem = (idx_t)(((uint64_t)cur * 10 + (ch - '0')) % n);
            if (!visited[new_rem]) {
                visited[new_rem] = true;
                parent[new_rem] = cur;
                digit[new_rem] = ch;
                if (new_rem == 0) {            /* minimal multiple found */
                    /* Count digits of the number. */
                    size_t len = 0;
                    idx_t p = new_rem;
                    while (true) {
                        ++len;
                        if (parent[p] == ROOT) break;
                        p = parent[p];
                    }

                    /* Reconstruct the decimal string. */
                    char *num_str = (char*)malloc(len + 1);
                    if (!num_str) {
                        free(queue); free(visited); free(parent); free(digit);
                        return (seq_t)0;
                    }
                    p = new_rem;
                    for (size_t i = 0; i < len; ++i) {
                        num_str[len - 1 - i] = digit[p];
                        if (parent[p] == ROOT) break;
                        p = parent[p];
                    }
                    num_str[len] = '\0';

                    /* Compute the multiple as a seq_t value. */
                    seq_t m = 0;
                    for (size_t i = 0; i < len; ++i)
                        m = m * (seq_t)10 + (seq_t)(num_str[i] - '0');

                    seq_t result = m / (seq_t)n;

                    free(num_str);
                    free(queue); free(visited); free(parent); free(digit);
                    return result;
                }
                queue[tail++] = new_rem;
            }
        }
    }

    /* Should never happen for n >= 1. */
    free(queue); free(visited); free(parent); free(digit);
    return (seq_t)0;
}
