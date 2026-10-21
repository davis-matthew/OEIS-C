/*
 * A078248  Smallest multiple of n using only digits 0 and 9.
 * https://oeis.org/A078248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest positive integer consisting of only digits 0 and
 *            9 that is divisible by n
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 108, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence */
    if (n <= 1) {
        return (seq_t)9;
    }

    /* If 9 itself is divisible by n we are done */
    idx_t start = (idx_t)((seq_t)9 % (seq_t)n);
    if (start == 0) {
        return (seq_t)9;
    }

    /* Memory for BFS */
    idx_t *parent  = (idx_t*)      malloc((size_t)n * sizeof(idx_t));
    unsigned char *digit = (unsigned char*)malloc((size_t)n * sizeof(unsigned char));
    unsigned char *visited = (unsigned char*)calloc((size_t)n, sizeof(unsigned char));
    idx_t *queue = (idx_t*)malloc((size_t)n * sizeof(idx_t));

    if (!parent || !digit || !visited || !queue) {
        /* Allocation failure – should not happen in the test harness */
        return (seq_t)0;
    }

    /* Initialize BFS */
    visited[start] = 1;
    parent[start] = start;   /* root has itself as parent */
    digit[start] = 9;
    int front = 0, back = 1;
    queue[0] = start;
    int found = 0;

    while (front < back && !found) {
        idx_t r = queue[front++];

        /* Append digit 0 */
        seq_t tmp0 = ((seq_t)r * (seq_t)10) % (seq_t)n;
        idx_t nr0 = (idx_t)tmp0;
        if (!visited[nr0]) {
            visited[nr0] = 1;
            parent[nr0] = r;
            digit[nr0] = 0;
            if (nr0 == 0) { found = 1; break; }
            queue[back++] = nr0;
        }

        /* Append digit 9 */
        seq_t tmp9 = (((seq_t)r * (seq_t)10) + (seq_t)9) % (seq_t)n;
        idx_t nr9 = (idx_t)tmp9;
        if (!visited[nr9]) {
            visited[nr9] = 1;
            parent[nr9] = r;
            digit[nr9] = 9;
            if (nr9 == 0) { found = 1; break; }
            queue[back++] = nr9;
        }
    }

    /* Reconstruct the number from the chain of digits (least‑significant first) */
    unsigned char *rev = (unsigned char*)malloc((size_t)n * sizeof(unsigned char));
    idx_t revlen = 0;
    idx_t cur = 0;
    while (1) {
        rev[revlen++] = digit[cur];
        if (cur == parent[cur]) {        /* reached the starting remainder */
            break;
        }
        cur = parent[cur];
    }

    /* Build the seq_t value by multiplying by 10 and adding the digits in reverse order */
    seq_t ans = 0;
    for (idx_t i = revlen; i > 0; --i) {
        ans = ans * (seq_t)10 + (seq_t)rev[i-1];
    }

    /* Free temporary storage */
    free(parent);
    free(digit);
    free(visited);
    free(queue);
    free(rev);

    return ans;
}
