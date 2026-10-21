/*
 * A096687  Least k such that decimal representation of k*n contains only
 *          digits 0 and 8.
 * https://oeis.org/A096687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A078247(n) / n, where A078247(n) is the smallest positive
 *            integer consisting only of the digits 0 and 8 that is divisible
 *            by n. The function implements a breadth‑first search over the
 *            remainders modulo n
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 26, u64 n <= 998, u128 n <= 9998,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The build system supplies seq_t (unsigned or arbitrary‑precision) and
   idx_t (uint64_t).  The function is written so that it is valid C11
   and C++17 alike. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

seq_t a(idx_t n)
{
    if (n == 0) {          /* undefined for offset 1 */
        return 0;
    }

    /* If 8 is already a multiple of n, the answer is 8/n. */
    idx_t start_rem = (unsigned __int128)8 % n;
    if (start_rem == 0) {
        return (seq_t)(8 / n);
    }

    /* Memory allocation with casts to keep the code valid in C++. */
    idx_t *parent   = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    uint8_t *digit  = (uint8_t *)malloc((size_t)n * sizeof(uint8_t));
    bool   *seen    = (bool *)calloc((size_t)n, sizeof(bool));
    if (!parent || !digit || !seen) {
        free(parent); free(digit); free(seen);
        return 0;
    }
    idx_t *queue = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!queue) {
        free(parent); free(digit); free(seen);
        return 0;
    }

    size_t head = 0, tail = 0;

    /* Initialise BFS with the first digit "8". */
    parent[start_rem] = UINT64_MAX;      /* sentinel */
    digit[start_rem]  = 8;
    seen[start_rem]   = true;
    queue[tail++]     = start_rem;

    idx_t found_rem = UINT64_MAX;

    /* BFS exploring appending 0 then 8: smaller numerically first. */
    while (head < tail) {
        idx_t rem = queue[head++];
        if (rem == 0) {
            found_rem = rem;
            break;
        }
        for (int di = 0; di < 2; ++di) {
            uint8_t d = (di == 0) ? 0 : 8;
            idx_t newrem = ((unsigned __int128)rem * 10u + d) % n;
            if (!seen[newrem]) {
                seen[newrem]   = true;
                parent[newrem] = rem;
                digit[newrem]  = d;
                queue[tail++]  = newrem;
            }
        }
    }

    /* In the very unlikely event that no such number exists, return 0. */
    if (found_rem == UINT64_MAX) {
        free(parent); free(digit); free(seen); free(queue);
        return 0;
    }

    /* Reconstruct the digits of the found number. */
    uint8_t *rev_digits = (uint8_t *)malloc((size_t)n * sizeof(uint8_t));
    if (!rev_digits) {
        free(parent); free(digit); free(seen); free(queue);
        return 0;
    }
    size_t pos = 0;
    idx_t cur = found_rem;
    while (cur != UINT64_MAX) {
        rev_digits[pos++] = digit[cur];
        cur = parent[cur];
    }

    /* Build the number as seq_t, then divide by n to get k. */
    seq_t x = 0;
    for (int i = (int)pos - 1; i >= 0; --i) {
        x = x * 10 + (seq_t)rev_digits[i];
    }
    seq_t k = x / (seq_t)n;

    /* Clean up. */
    free(parent); free(digit); free(seen); free(queue); free(rev_digits);

    return k;
}
