/*
 * A086342  Smallest number of 1's in binary expansion of any positive
 *          multiple of n.
 * https://oeis.org/A086342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4864, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A086342 – Smallest number of 1's in binary expansion of any positive multiple of n
   Computation via 0‑1 BFS on the congruence classes modulo n. */
#include <stdlib.h>
#include <stdbool.h>

/* external linkage is required */
seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0) return 0;

    /* powers of two have a(n) = 1 – the algorithm will give the same result,
       but an explicit test avoids a trivial case that would mis‑behave the
       0‑1 BFS (starting from remainder 1). */
    if (n == 1) return 1;

    idx_t m = n;

    /* distance (minimal popcount) to each remainder 0 … n-1 */
    seq_t *dist = (seq_t *)malloc(m * sizeof(seq_t));
    if (!dist) return 0;          /* allocation failed – return 0 as a safe value */

    for (idx_t i = 0; i < m; ++i) dist[i] = (seq_t)(m + 1);   /* infinity */

    /* simple circular double‑ended queue for 0‑1 BFS */
    idx_t dqsize = 2 * m + 5;     /* enough room for all pushes */
    int   *dq    = (int *)malloc(dqsize * sizeof(int));
    idx_t head = 0, tail = 0;     /* empty */

    /* start from remainder 1 (number 1) – guarantees the path has at least one
       '1' bit in the resulting multiple.  */
    idx_t start = 1 % n;
    dist[start] = 1;
    dq[tail++] = (int)start;
    if (tail >= dqsize) tail -= dqsize;

    while (head != tail) {
        /* pop front */
        idx_t r = (idx_t)dq[head];
        ++head;
        if (head >= dqsize) head -= dqsize;

        seq_t d = dist[r];

        /* transition by appending a '0' bit – cost 0 */
        idx_t r0 = (r << 1) % n;
        if (dist[r0] > d) {
            dist[r0] = d;
            head = (head == 0) ? dqsize - 1 : head - 1;  /* push front */
            dq[head] = (int)r0;
        }

        /* transition by appending a '1' bit – cost 1 */
        idx_t r1 = ((r << 1) + 1) % n;
        if (dist[r1] > d + 1) {
            dist[r1] = d + 1;
            dq[tail] = (int)r1;                          /* push back */
            ++tail;
            if (tail >= dqsize) tail -= dqsize;
        }
    }

    seq_t result = dist[0];
    free(dist);
    free(dq);
    return result;
}
