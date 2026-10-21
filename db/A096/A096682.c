/*
 * A096682  Least k such that decimal representation of k*n contains only
 *          digits 0 and 3.
 * https://oeis.org/A096682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   in this implementation we compute a(n) = the smallest positive
 *            integer k such that k*n consists only of the digits 0 and 3. We
 *            find that integer by a breadth‑first search on the remainders of
 *            numbers composed from the alphabet {0,3}. This yields the
 *            minimal multiple of n made of these digits; div...
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 296, u64 n <= 2996,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) { /* undefined, but return 0 to avoid UB */
        return 0;
    }

    /* Arrays to keep breadth‑first search state */
    bool   *visited = (bool*)   malloc(n * sizeof(bool));
    idx_t  *prev    = (idx_t*)  malloc(n * sizeof(idx_t));
    unsigned char *digit = (unsigned char*) malloc(n * sizeof(unsigned char));
    if (!visited || !prev || !digit) {
        /* out of memory – return 0 as a safe fallback */
        if (visited) free(visited);
        if (prev)    free(prev);
        if (digit)   free(digit);
        return 0;
    }
    memset(visited, 0, n * sizeof(bool));

    /* queue for BFS, implemented as a simple ring buffer */
    idx_t *queue = (idx_t*) malloc(n * sizeof(idx_t));
    if (!queue) {
        free(visited); free(prev); free(digit);
        return 0;
    }
    idx_t head = 0, tail = 0;
    /* start with the single digit 3 (cannot start with 0) */
    idx_t r0 = 3 % n;
    visited[r0] = true;
    prev[r0]    = (idx_t)-1;      /* sentinel for root */
    digit[r0]   = 3;
    queue[tail++] = r0;

    idx_t foundrem = (idx_t)-1;
    while (head != tail) {
        idx_t r = queue[head++];
        if (r == 0) {                 /* found the required remainder */
            foundrem = r;
            break;
        }
        /* try appending digits 0 and 3 */
        for (int d = 0; d < 2; ++d) {
            unsigned char dig = (d == 0 ? 0 : 3);
            idx_t newr = (idx_t)(( (unsigned __int128)r * 10 + dig) % n);
            if (!visited[newr]) {
                visited[newr] = true;
                prev[newr] = r;
                digit[newr] = dig;
                queue[tail++] = newr;
            }
        }
    }

    if (foundrem == (idx_t)-1) {
        /* No solution found – should not happen for valid n */
        free(visited); free(prev); free(digit); free(queue);
        return 0;
    }

    /* Reconstruct the digits of the smallest multiple from remainder 0 */
    char *digits = (char*) malloc(n + 1);  /* +1 for safety */
    size_t pos = 0;
    idx_t cur = foundrem;  /* this will be 0 */
    while (cur != (idx_t)-1) {
        digits[pos++] = digit[cur];
        cur = prev[cur];
    }
    /* Reverse to get the correct order (most significant first) */
    for (size_t i = 0; i < pos / 2; ++i) {
        char tmp = digits[i];
        digits[i] = digits[pos - 1 - i];
        digits[pos - 1 - i] = tmp;
    }

    /* Build the number M from its decimal digits */
    seq_t M = 0;
    for (size_t i = 0; i < pos; ++i) {
        M = M * 10 + digits[i];
    }

    /* Compute a(n) = M / n */
    seq_t result = M / (seq_t)n;

    /* Clean up */
    free(visited);
    free(prev);
    free(digit);
    free(queue);
    free(digits);

    return result;
}
