/*
 * A216478  a(n) is the least multiple of n which uses only digits 1 and 2.
 *          a(n) = -1 if no such multiple exists.
 * https://oeis.org/A216478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 38, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the smallest multiple of n composed only of decimal
   digits 1 and 2, or -1 if no such multiple exists. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1; /* undefined for n = 0 */

    size_t mod = (size_t)n;

    bool  *visited = (bool  *)calloc(mod, sizeof(bool));
    int   *parent  = (int   *)calloc(mod, sizeof(int));
    char  *digit   = (char  *)calloc(mod, sizeof(char));
    size_t *queue  = (size_t *)malloc(mod * sizeof(size_t));
    if (!visited || !parent || !digit || !queue) {
        free(visited); free(parent); free(digit); free(queue);
        return (seq_t)-1;
    }

    size_t head = 0, tail = 0;
    const char digits[2] = {'1', '2'};

    /* initial digits */
    for (int di = 0; di < 2; ++di) {
        char dCh = digits[di];
        size_t r = (dCh - '0') % mod;
        if (r == 0) {
            free(visited); free(parent); free(digit); free(queue);
            return (seq_t)(dCh - '0');
        }
        if (!visited[r]) {
            visited[r] = true;
            parent[r] = -1;
            digit[r]  = dCh;
            queue[tail++] = r;
        }
    }

    bool found = false;
    while (head < tail && !found) {
        size_t cur = queue[head++];
        for (int di = 0; di < 2 && !found; ++di) {
            char dCh = digits[di];
            size_t next = (cur * 10 + (dCh - '0')) % mod;
            if (!visited[next]) {
                visited[next] = true;
                parent[next] = (int)cur;
                digit[next]  = dCh;
                if (next == 0) {
                    found = true;
                } else {
                    queue[tail++] = next;
                }
            }
        }
    }

    seq_t result = (seq_t)-1;
    if (found) {
        /* reconstruct number from remainder 0 backwards */
        size_t cap = 64;
        char *rev = (char *)malloc(cap);
        size_t revlen = 0;
        size_t cur = 0;
        while (1) {
            if (revlen >= cap) {
                cap <<= 1;
                rev = (char *)realloc(rev, cap);
            }
            rev[revlen++] = digit[cur];
            int p = parent[cur];
            if (p == -1) break;
            cur = (size_t)p;
        }

        char *buf = (char *)malloc(revlen + 1);
        for (size_t i = 0; i < revlen; ++i)
            buf[i] = rev[revlen - 1 - i];
        buf[revlen] = '\0';

        result = seq_from_str(buf);

        free(buf);
        free(rev);
    }

    free(visited);
    free(parent);
    free(digit);
    free(queue);
    return result;
}
