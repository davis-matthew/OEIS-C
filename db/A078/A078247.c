/*
 * A078247  Smallest multiple of n using only digits 0 and 8.
 * https://oeis.org/A078247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 26, u64 n <= 998, u128 n <= 998,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation finds the smallest multiple of n that consists only
   of the digits 0 and 8.  It performs a breadth‑first search on the
   residues modulo n.  Each visited residue r represents the remainder of
   some partial number.  From r we can append 0 or 8, producing the new
   residues (r*10) % n and (r*10+8) % n.  When residue 0 is reached,
   the corresponding partial number is divisible by n and is the
   minimum number with these properties (shortest length, lexicographically
   smallest).  The algorithm works for all n ≥ 1, for any seq_t type
   supplied by the build. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle very small n without allocating large arrays. */
    if (n == 0) return (seq_t)0;            /* undefined, but safe guard */

    /* n >= 1 */
    size_t m = (size_t)n;                   /* size for arrays (n is small in tests) */

    /* Arrays for BFS: visited flag, parent residue, and digit used to reach a residue. */
    char *vis  = (char *)calloc(m, sizeof(char));
    idx_t *par = (idx_t *)malloc(m * sizeof(idx_t));
    char *dig  = (char *)calloc(m, sizeof(char));

    if (!vis || !par || !dig) {
        /* Allocation failed – return zero as a safe fallback. */
        free(vis); free(par); free(dig);
        return (seq_t)0;
    }

    /* initialise parents to an invalid sentinel (n itself). */
    for (size_t i = 0; i < m; ++i) par[i] = (idx_t)n;

    /* The starting remainder after the first digit '8'. */
    idx_t start = 8 % n;

    /* If the first digit already gives a solution, return immediately. */
    if (start == 0) {
        free(vis); free(par); free(dig);
        return (seq_t)8;
    }

    /* Queue for BFS: ring buffer of size n. */
    idx_t *queue = (idx_t *)malloc(m * sizeof(idx_t));
    if (!queue) {
        free(vis); free(par); free(dig);
        return (seq_t)0;
    }
    size_t qh = 0, qt = 0;

    /* Initialise BFS with the residue from “8”. */
    vis[start] = 1;
    par[start] = (idx_t)n;   /* sentinel for root */
    dig[start] = '8';
    queue[qt++] = start;

    idx_t found = (idx_t)n;  /* sentinel for not found */

    /* Breadth‑first search on residues. */
    while (qh < qt) {
        idx_t cur = queue[qh++];
        if (cur == 0) {            /* divisible by n */
            found = cur;
            break;
        }

        /* Append digit 0. */
        idx_t r0 = (cur * 10) % n;
        if (!vis[r0]) {
            vis[r0] = 1;
            par[r0] = cur;
            dig[r0] = '0';
            queue[qt++] = r0;
        }

        /* Append digit 8. */
        idx_t r8 = (cur * 10 + 8) % n;
        if (!vis[r8]) {
            vis[r8] = 1;
            par[r8] = cur;
            dig[r8] = '8';
            queue[qt++] = r8;
        }
    }

    /* If we never reach remainder 0 (theoretically impossible for n>0
       because numbers ending with 0 are always divisible by 5 and any
       even number is divisible by 2), return 0. */
    if (found == (idx_t)n) {
        free(vis); free(par); free(dig); free(queue);
        return (seq_t)0;
    }

    /* Reconstruct the digits of the answer by backtracking from residue 0. */
    /* The maximum length is at most n, which is small. */
    char stack[1024];
    size_t len = 0;
    idx_t r = 0;
    while (par[r] != (idx_t)n) {   /* until we reach the root residue */
        stack[len++] = dig[r];
        r = par[r];
    }
    /* Add the root digit (which is '8'). */
    stack[len++] = dig[r];          /* root's digit is '8' */

    /* Now stack[0..len-1] holds the digits in reverse order: last digit first. */
    /* Build the seq_t value by traversing from the most significant digit. */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        char ch = stack[len - 1 - i];     /* most significant first */
        result = result * 10 + (seq_t)(ch - '0');
    }

    /* Clean up heap memory. */
    free(vis);
    free(par);
    free(dig);
    free(queue);

    return result;
}
