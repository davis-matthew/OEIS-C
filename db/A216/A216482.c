/*
 * A216482  a(n) is the least value of k such that k*n uses only digits 1 and
 *          2. a(n) = -1 if no such multiple exists.
 * https://oeis.org/A216482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) = least k such that k*n uses only digits 1 and 2. This
 *            implementation finds the smallest multiple of n composed of
 *            digits 1 and 2 using a breadth‑first search on remainders. The
 *            found multiple m is then divided by n to obtain the multiplier k
 * Range:     i8 n <= 8, i16 n <= 98, i32 n <= 988, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;          /* undefined, keep safe */

    /* Convert to int for array indices; problem guarantees n is small. */
    int N = (int)n;

    /* BFS over remainders modulo n. */
    bool *visited = (bool*)malloc(N * sizeof(bool));
    int  *parent   = (int*) malloc(N * sizeof(int));
    uint8_t *dgt   = (uint8_t*)malloc(N * sizeof(uint8_t));
    int *queue = (int*) malloc(N * sizeof(int));
    if (!visited || !parent || !dgt || !queue)
    {
        free(visited); free(parent); free(dgt); free(queue);
        return (seq_t)-1;                     /* out of memory */
    }

    for (int i = 0; i < N; ++i) visited[i] = false;

    int head = 0, tail = 0;
    int target_rem = -1;                      /* remainder 0 if found */

    /* Start with digits 1 and 2. */
    for (int d = 1; d <= 2; ++d)
    {
        int rem = d % n;
        if (!visited[rem])
        {
            visited[rem] = true;
            parent[rem] = -1;      /* root has no parent */
            dgt[rem] = (uint8_t)d;
            queue[tail++] = rem;
            if (rem == 0) { target_rem = rem; break; }
        }
    }

    /* Main BFS loop. */
    while (head < tail && target_rem == -1)
    {
        int cur = queue[head++];
        for (int d = 1; d <= 2; ++d)
        {
            int nxt = (int)(((uint64_t)cur * 10 + d) % n);
            if (!visited[nxt])
            {
                visited[nxt] = true;
                parent[nxt] = cur;
                dgt[nxt] = (uint8_t)d;
                queue[tail++] = nxt;
                if (nxt == 0) { target_rem = nxt; break; }
            }
        }
    }

    if (target_rem == -1) /* no such multiple */
    {
        free(visited); free(parent); free(dgt); free(queue);
        return (seq_t)-1;
    }

    /* Reconstruct the digits of m (the minimal multiple). */
    int maxlen = tail;          /* at most N digits */
    uint8_t *digits = (uint8_t*)malloc(maxlen * sizeof(uint8_t));
    if (!digits) { free(visited); free(parent); free(dgt); free(queue); return (seq_t)-1; }

    int len = 0;
    int cur = target_rem;       /* 0 remainder */
    while (cur != -1)
    {
        digits[len++] = dgt[cur];
        cur = parent[cur];
    }
    /* digits are now in reverse order; reverse them. */
    for (int i = 0; i < len/2; ++i)
    {
        uint8_t tmp = digits[i];
        digits[i] = digits[len-1-i];
        digits[len-1-i] = tmp;
    }

    /* Compute k = m / n via long division, storing result as seq_t. */
    seq_t k = 0;
    idx_t rem = 0;
    for (int i = 0; i < len; ++i)
    {
        rem = rem * 10 + digits[i];
        idx_t q = rem / n;
        k = k * 10 + (seq_t)q;
        rem = rem % n;
    }

    /* Clean up. */
    free(visited); free(parent); free(dgt); free(queue); free(digits);

    return k;
}
