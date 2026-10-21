/*
 * A078240  a(n) = smallest multiple of n using only composite digits
 *          (0,4,6,8,9).
 * https://oeis.org/A078240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 358, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function implements A078240: smallest positive multiple of n whose
 * decimal digits belong to the set {0,4,6,8,9}.  It performs a breadth‑first
 * search on the remainder graph modulo n; the first time a remainder 0 is
 * reached gives the desired number.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static const unsigned char ALLOWED_DIGITS[5] = {0, 4, 6, 8, 9};

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n = 0 */

    size_t N = (size_t)n;               /* number of possible remainders */

    /* Allocate the BFS data structures.  Use malloc in case n is large. */
    int64_t *prev   = (int64_t *)malloc(N * sizeof(int64_t));
    unsigned char *digit   = (unsigned char *)malloc(N * sizeof(unsigned char));
    bool   *visited = (bool *)malloc(N * sizeof(bool));
    size_t *queue   = (size_t *)malloc(N * sizeof(size_t));

    if (!prev || !digit || !visited || !queue) {
        free(prev); free(digit); free(visited); free(queue);
        return 0;
    }

    for (size_t i = 0; i < N; ++i) {
        visited[i] = false;
        prev[i]    = -1;
    }

    size_t head = 0, tail = 0;

    /* Initialise frontier with one‑digit numbers (no leading 0). */
    for (int d = 1; d <= 9; ++d) {
        if (d == 4 || d == 6 || d == 8 || d == 9) {
            size_t r = (size_t)d % N;
            if (!visited[r]) {
                visited[r] = true;
                prev[r]    = -1;
                digit[r]   = (unsigned char)d;
                queue[tail++] = r;
                if (r == 0) goto construct_number;
            }
        }
    }

    /* BFS on remainder graph. */
    while (head < tail) {
        size_t cur = queue[head++];
        for (int di = 0; di < 5; ++di) {
            unsigned char d = ALLOWED_DIGITS[di];
            size_t nxt = (cur * 10 + d) % N;
            if (!visited[nxt]) {
                visited[nxt] = true;
                prev[nxt]    = (int64_t)cur;
                digit[nxt]   = d;
                queue[tail++] = nxt;
                if (nxt == 0) goto construct_number;
            }
        }
    }

    /* No solution (should not happen for the provided digit set). */
    free(prev); free(digit); free(visited); free(queue);
    return 0;

construct_number:
    /* Reconstruct number from predecessors. */
    unsigned char buf[512];          /* buffer for digits, plenty for n <= 10^4 */
    size_t pos = 0;
    int64_t r = 0;
    while (r != -1) {
        buf[pos++] = digit[r];
        r = prev[r];
    }

    /* Convert digit buffer to seq_t value. */
    seq_t result = 0;
    while (pos--) {
        result = result * 10 + (seq_t)buf[pos];
    }

    free(prev); free(digit); free(visited); free(queue);
    return result;
}
