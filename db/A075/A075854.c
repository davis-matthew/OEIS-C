/*
 * A075854  Smallest integer composed of digits 1..n such that no subsequence
 *          is repeated and every possible subsequence of length 2 appears.
 * https://oeis.org/A075854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   produce the lexicographically smallest de Bruijn sequence of
 *            order 2 over the alphabet {1,…,n} and interpret it as a decimal
 *            integer. The algorithm follows Hierholzer’s method for Eulerian
 *            paths in the complete directed graph of n vertices. The
 *            resulting sequence has length n²+1 digits and starts...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* n is guaranteed ≥1 and reasonably small (≤ ~10000)   */
    int nn = (int)n;             /* cast to int for array indexing   */
    size_t edges = (size_t)nn * nn;   /* number of directed edges        */
    /* stack for Hierholzer’s algorithm */
    int *stack = (int *)malloc((edges + 2) * sizeof(int));
    size_t top = 0;                       /* stack size */
    stack[top++] = 1;                     /* start at vertex 1 */

    /* next unused outgoing edge index for each vertex (0…nn-1) */
    int *next_edge = (int *)calloc(nn + 2, sizeof(int));

    /* array to collect vertices in reverse order of the circuit */
    int *rev = (int *)malloc((edges + 2) * sizeof(int));
    size_t rev_cnt = 0;

    while (top > 0) {
        int v = stack[top - 1];
        if (next_edge[v] < nn) {          /* still edges from v   */
            int w = next_edge[v] + 1;     /* destination vertex  */
            ++next_edge[v];
            stack[top++] = w;             /* push w onto stack   */
        } else {                          /* no more edges: pop   */
            rev[rev_cnt++] = v;
            --top;
        }
    }

    /* reverse rev to obtain the forward order of vertices */
    for (size_t i = 0; i < rev_cnt / 2; ++i) {
        int tmp = rev[i];
        rev[i] = rev[rev_cnt - 1 - i];
        rev[rev_cnt - 1 - i] = tmp;
    }

    /* build the integer value: multiply by 10 and add each digit  */
    seq_t value = 0;
    for (size_t i = 0; i < rev_cnt; ++i) {
        value = value * 10 + (seq_t)rev[i];
    }

    free(stack);
    free(next_edge);
    free(rev);
    return value;
}
