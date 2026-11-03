/*
 * A096681  Least k such that decimal representation of k*n contains only
 *          digits 0 and 2.
 * https://oeis.org/A096681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = least k such that decimal representation of k*n *
 *            consists only of digits 0 and 2. * The value returned is k =
 *            (smallest such number)/n
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 98, u64 n <= 998,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9934 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Helper: construct the smallest decimal number made of digits 0 and 2
 * that is divisible by n. Return the value of that number as seq_t. */
static seq_t smallest_02_multiple(idx_t n)
{
    /* Special case: n divides 2 */
    if (2 % n == 0)
        return (seq_t)2;

    size_t N = (size_t)n;
    idx_t *prev = (idx_t *)malloc(N * sizeof(idx_t));
    uint8_t *digit = (uint8_t *)malloc(N * sizeof(uint8_t));
    bool *vis = (bool *)malloc(N * sizeof(bool));
    idx_t *q = (idx_t *)malloc(N * sizeof(idx_t));
    if (!prev || !digit || !vis || !q) {
        /* In case of allocation failure, return 0 (should not happen). */
        free(prev); free(digit); free(vis); free(q);
        return (seq_t)0;
    }

    for (size_t i = 0; i < N; ++i) vis[i] = false;

    idx_t r_start = 2 % n;
    vis[r_start] = true;
    prev[r_start] = n;          /* sentinel for start */
    digit[r_start] = 2;
    size_t head = 0, tail = 0;
    q[tail++] = r_start;

    idx_t found = 0;
    while (head < tail) {
        idx_t r = q[head++];
        if (r == 0) { found = 0; break; }

        /* Append digit 0 then 2 */
        for (uint8_t d = 0; d <= 2; d += 2) {
            idx_t nr = (r * 10 + d) % n;
            if (!vis[nr]) {
                vis[nr] = true;
                prev[nr] = r;
                digit[nr] = d;
                q[tail++] = nr;
            }
        }
    }

    /* Back‑track to reconstruct the number */
    size_t len = 0;
    idx_t r = found;
    uint8_t *rev = (uint8_t *)malloc((N + 1) * sizeof(uint8_t));
    if (!rev) {
        free(prev); free(digit); free(vis); free(q);
        return (seq_t)0;
    }

    while (r != r_start) {
        rev[len++] = digit[r];
        r = prev[r];
    }
    rev[len++] = digit[r_start];   /* the leading 2 */

    /* Build the number from most significant digit to least */
    seq_t num = (seq_t)0;
    for (size_t i = 0; i < len; ++i) {
        num = num * 10 + rev[len - 1 - i];
    }

    free(prev); free(digit); free(vis); free(q); free(rev);
    return num;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined, but guard */

    seq_t rep = smallest_02_multiple(n);
    seq_t k = rep / n;               /* exact division */
    return k;
}
