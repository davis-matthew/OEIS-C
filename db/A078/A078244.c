/*
 * A078244  Smallest multiple of n using only digits 0 and 5.
 * https://oeis.org/A078244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 17, u64 n <= 395, u128 n <= 998,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the smallest positive integer composed only of the digits 0 and 5
 * that is divisible by a given positive integer n.
 *
 * The algorithm performs a breadth‑first search on the graph of possible
 * remainders modulo n.  Each state is a remainder r < n, and from r we
 * can transition to (r*10+0) % n and (r*10+5) % n by appending the
 * corresponding digit.  The search starts with the remainder of the
 * single digit 5.  The first time we reach remainder 0 the associated
 * path of digits gives the desired number.  The path is then reconstructed
 * and evaluated as a seq_t value using normal arithmetic operators
 * (+, *, %).
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined case – returned for completeness */
    /* The remainder after appending the initial digit '5'. */
    idx_t start_rem = (idx_t)(5 % n);
    if (start_rem == 0) return (seq_t)5;   /* n divides 5 */

    /* Arrays used by the BFS.  Size n is sufficient because at most n
     * distinct remainders exist. */
    bool   *visited = (bool*)calloc((size_t)n, sizeof(bool));
    idx_t  *parent  = (idx_t*)malloc((size_t)n * sizeof(idx_t));
    unsigned char *add = (unsigned char*)malloc((size_t)n * sizeof(unsigned char));
    if (!visited || !parent || !add) {
        /* In case memory allocation fails – return 0 as a sentinel. */
        return 0;
    }

    /* Queue implemented as a simple circular buffer. */
    idx_t *queue = (idx_t*)malloc((size_t)n * sizeof(idx_t));
    idx_t q_head = 0, q_tail = 0;

    /* Initialize BFS with the first digit 5. */
    visited[start_rem] = true;
    parent[start_rem] = (idx_t)n;    /* sentinel for the start state */
    add[start_rem] = 5;
    queue[q_tail++] = start_rem;

    idx_t found_rem = 0;             /* will hold the final remainder 0 */

    while (q_head < q_tail) {
        idx_t cur = queue[q_head++];
        /* Try appending 0 and 5. */
        for (int i = 0; i < 2; ++i) {
            unsigned char d = (i == 0) ? 0 : 5;
            /* Compute new remainder (use 128‑bit to avoid overflow for
               arbitrary n). */
            idx_t new_rem = (idx_t)(((__int128)cur * 10 + d) % n);
            if (!visited[new_rem]) {
                visited[new_rem] = true;
                parent[new_rem] = cur;
                add[new_rem] = d;
                queue[q_tail++] = new_rem;
                if (new_rem == 0) {
                    found_rem = new_rem;
                    goto reconstruct;
                }
            }
        }
    }

reconstruct:
    /* Determine the length of the number.  The path starts with the
       initial digit 5 followed by each digit stored on the way to 0. */
    idx_t len = 1;   /* for the initial 5 */
    for (idx_t r = found_rem; r != start_rem; r = parent[r]) ++len;

    /* Allocate a temporary array to hold the digits in order. */
    unsigned char *digits = (unsigned char*)malloc((size_t)len * sizeof(unsigned char));
    idx_t pos = 0;
    digits[pos++] = 5;   /* initial digit */
    for (idx_t r = found_rem; r != start_rem; r = parent[r]) {
        digits[pos++] = add[r];
    }
    /* Reverse the collected appended digits (they were collected backwards). */
    for (idx_t i = 1, j = len - 1; i < j; ++i, --j) {
        unsigned char tmp = digits[i];
        digits[i] = digits[j];
        digits[j] = tmp;
    }

    /* Build the result as a seq_t. */
    seq_t result = 0;
    for (idx_t i = 0; i < len; ++i) {
        result = result * 10 + (seq_t)digits[i];
    }

    /* Clean up. */
    free(visited);
    free(parent);
    free(add);
    free(queue);
    free(digits);

    return result;
}
