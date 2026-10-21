/*
 * A096685  Least k such that decimal representation of k*n contains only
 *          digits 0 and 6.
 * https://oeis.org/A096685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A078245(n)/n * Computes the least k such that k*n has
 *            decimal digits only 0 or 6 * and returns k. A078245(n) is the
 *            minimal number with that property
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n) {
    /* ------------------------------------------------------------ */
    /* 1. Edge case: n ≥ 1 is guaranteed by the problem statement. */
    /* ------------------------------------------------------------ */
    if (n == 0) return (seq_t)0;   /* defensive, should never happen */

    /* ------------------------------------------------------------ */
    /* 2. Prepare data structures for a BFS on remainders modulo n */
    /* ------------------------------------------------------------ */
    idx_t mask = n;   /* sentinel value for "no parent"      */
    bool *visited = (bool*)malloc((size_t)n * sizeof(bool));
    idx_t *parent  = (idx_t*)malloc((size_t)n * sizeof(idx_t));
    unsigned char *last_digit = (unsigned char*)malloc((size_t)n * sizeof(unsigned char));
    idx_t *queue  = (idx_t*)malloc((size_t)n * sizeof(idx_t));
    if (!visited || !parent || !last_digit || !queue) {
        /* Allocation failure – return 0 as a safe fallback. */
        free(visited); free(parent); free(last_digit); free(queue);
        return (seq_t)0;
    }
    for (idx_t i = 0; i < n; ++i) { visited[i] = false; parent[i] = mask; }

    /* ------------------------------------------------------------ */
    /* 3. BFS start with the single digit '6' */
    /* ------------------------------------------------------------ */
    idx_t start = 6 % n;
    visited[start] = true;
    parent[start] = mask;          /* no parent; this is the first digit */
    last_digit[start] = 6;
    idx_t head = 0, tail = 0;
    queue[tail++] = start;

    /* ------------------------------------------------------------ */
    /* 4. BFS loop – explore digits 0 and then 6 to ensure minimality */
    /* ------------------------------------------------------------ */
    bool found = (start == 0);
    while (!found && head < tail) {
        idx_t cur = queue[head++];
        for (int d = 0; d < 2; ++d) {
            unsigned char dig = (d ? 6 : 0);
            idx_t nxt = (cur * 10u + dig) % n;
            if (!visited[nxt]) {
                visited[nxt] = true;
                parent[nxt] = cur;
                last_digit[nxt] = dig;
                queue[tail++] = nxt;
                if (nxt == 0) { found = true; break; }
            }
        }
    }

    /* ------------------------------------------------------------ */
    /* 5. Reconstruct the minimal number (as digits in a buffer)   */
    /* ------------------------------------------------------------ */
    /* The number of digits is at most the number of visited states,
       which is ≤ n. Allocate buffers accordingly. */
    unsigned char *temp = (unsigned char*)malloc((size_t)n * sizeof(unsigned char));
    if (!temp) {
        free(visited); free(parent); free(last_digit); free(queue);
        return (seq_t)0;
    }

    size_t len = 0;
    idx_t cur = 0;
    /* Walk back from remainder 0 to the start. */
    while (cur != mask) {
        temp[len++] = last_digit[cur];
        cur = parent[cur];
    }

    /* The digits are currently in reverse order (least significant first).
       We need them in normal order to form the decimal string. */
    char *num_str = (char*)malloc((len + 1) * sizeof(char));
    if (!num_str) {
        free(temp); free(visited); free(parent); free(last_digit); free(queue);
        return (seq_t)0;
    }
    for (size_t i = 0; i < len; ++i) {
        num_str[i] = (char)(temp[len - 1 - i] + '0');
    }
    num_str[len] = '\0';

    /* ------------------------------------------------------------ */
    /* 6. Convert the digit string to a seq_t value and divide by n */
    /* ------------------------------------------------------------ */
    seq_t X = seq_from_str(num_str);
    seq_t result = X / (seq_t)n;   /* integer division; seq_t supports this op */

    /* ------------------------------------------------------------ */
    /* 7. Clean up and return                                 */
    /* ------------------------------------------------------------ */
    free(temp); free(num_str);
    free(visited); free(parent); free(last_digit); free(queue);
    return result;
}
