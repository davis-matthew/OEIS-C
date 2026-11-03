/*
 * A034712  Largest number using only digits <= n that does not repeat a pair
 *          of consecutive digits.
 * https://oeis.org/A034712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = lexicographically maximal Eulerian trail of the *
 *            complete directed graph on vertices {0,…,n} (digits 0,...,n), *
 *            represented as an integer whose decimal digits are those
 *            vertices. * The algorithm constructs the trail by Hierholzer's
 *            method, * always taking the highest unused outgoing edge...
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

seq_t a(idx_t n)
{
    /* Length of the resulting integer in decimal digits:
     * (n+1)^2 + 1
     */
    size_t N = (size_t)(n + 1);
    size_t len = N * N + 1;
    if (len == 0) return (seq_t)0;   /* defensive for very large n */

    /* Buffers for the output digits and the Hierholzer stack. */
    char *digits = (char*)malloc(len);
    int  *stack  = (int*)malloc(len * sizeof(int));
    if (!digits || !stack) return (seq_t)0;   /* allocation failure */

    /* cur[v] holds the highest unused destination from vertex v. */
    int *cur = (int*)malloc((n + 1) * sizeof(int));
    if (!cur) { free(digits); free(stack); return (seq_t)0; }

    for (int i = 0; i <= (int)n; ++i) cur[i] = (int)n;

    size_t pos = len - 1;   /* position to write next digit (from end) */
    size_t sp  = 1;          /* stack pointer (number of elements) */
    stack[0] = (int)n;       /* start at the largest vertex */

    while (sp > 0) {
        int v = stack[sp - 1];
        if (cur[v] >= 0) {
            int u = cur[v];
            cur[v]--;          /* consume this edge */
            stack[sp++] = u;    /* push the destination */
        } else {
            /* No more unused edges from v: add v to result. */
            --sp;
            digits[pos--] = '0' + v;
        }
    }

    /* Convert the digit string to seq_t. */
    seq_t res = seq_from_str(digits);

    /* Free temporary buffers. */
    free(digits);
    free(stack);
    free(cur);

    return res;
}
