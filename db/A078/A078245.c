/*
 * A078245  Smallest multiple of n using only digits 0 and 6.
 * https://oeis.org/A078245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 80, u64 n <= 2375,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = smallest positive multiple of n made only of digits 0 and 6,
   in decimal. The algorithm performs a breadth–first search on remainders
   modulo n, using the allowed digits 0 and 6.  It visits each remainder at
   most once, so the time complexity per call is O(n). */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  Return 0 for an invalid argument. */
    if (n == 0)
        return 0;

    /* Work with the modulus as an idx_t; it will never exceed n–1 and fits
       into an unsigned 64‑bit integer.  Allocate all temporary structures
       with size n.  The constraints (n ≤ 10 000 for the test suite) make
       this memory usage safe. */
    size_t m = (size_t)n;

    idx_t *parent = (idx_t *)malloc(m * sizeof(idx_t));
    unsigned char *dig   = (unsigned char *)malloc(m * sizeof(unsigned char));
    unsigned char *seen   = (unsigned char *)malloc(m * sizeof(unsigned char));
    if (!parent || !dig || !seen) {
        free(parent); free(dig); free(seen);
        return 0;
    }
    for (idx_t i = 0; i < m; ++i)
        seen[i] = 0;

    /* Queue for the BFS, also sized n because at most n remainders exist. */
    idx_t *queue = (idx_t *)malloc(m * sizeof(idx_t));
    if (!queue) {
        free(parent); free(dig); free(seen);
        return 0;
    }
    size_t head = 0, tail = 0;

    /* Start from the single digit number 6.  Its remainder modulo n is
       6 % n. */
    idx_t start = (idx_t)(((seq_t)6) % (seq_t)n);
    parent[start] = (idx_t)-1;        /* root marker */
    dig[start] = 6;
    seen[start] = 1;
    queue[tail++] = start;

    /* BFS exploring edges labeled 0 and 6.  The '0' edge is enqueued before
       the '6' edge so that smaller numbers are found first. */
    while (head < tail) {
        idx_t cur = queue[head++];
        if (cur == 0)
            break;   /* found the minimal multiple */

        /* Append digit 0: new remainder = (cur * 10) % n. */
        idx_t next = (idx_t)(((seq_t)cur * 10) % (seq_t)n);
        if (!seen[next]) {
            seen[next] = 1;
            parent[next] = cur;
            dig[next] = 0;
            queue[tail++] = next;
        }

        /* Append digit 6: new remainder = (cur * 10 + 6) % n. */
        next = (idx_t)(((seq_t)cur * 10 + 6) % (seq_t)n);
        if (!seen[next]) {
            seen[next] = 1;
            parent[next] = cur;
            dig[next] = 6;
            queue[tail++] = next;
        }
    }

    /* Reconstruct the digit sequence from remainder 0 back to the start. */
    unsigned char *digits = (unsigned char *)malloc(m * sizeof(unsigned char));
    if (!digits) {
        free(parent); free(dig); free(seen); free(queue);
        return 0;
    }
    size_t dcnt = 0;
    idx_t cur = 0;
    do {
        digits[dcnt++] = dig[cur];
        cur = parent[cur];
    } while (cur != (idx_t)-1);     /* stop after the root (parent -1) */

    /* Convert the digit array (stored backwards) into a seq_t value. */
    seq_t value = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        unsigned char d = digits[dcnt - 1 - i];
        value = value * 10 + d;
    }

    /* Clean up. */
    free(parent);
    free(dig);
    free(seen);
    free(queue);
    free(digits);

    return value;
}
