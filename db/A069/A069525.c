/*
 * A069525  Smallest multiple of n with digit sum = 6, or 0 if no such number
 *          exists, e.g. a(9k)= 0.
 * https://oeis.org/A069525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest multiple of n whose digit sum equals 6, or 0
 *            if such a multiple does not exist
 * Range:     u8 n <= 12, u16 n <= 88, u32 n <= 528, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm performs a breadth‑first search over states (remainder,
   digit_sum).  Each state is represented by a linear index
   idx = sum * n + rem, with sum in [0,6] and 0 ≤ rem < n.  The total
   number of states is 7*n.  For each reachable state we store the parent
   state and the digit that was appended to reach it.  When a state with
   sum 6 and remainder 0 is found, the path from the root state to that
   state yields the required number.  This approach avoids keeping large
   arbitrary‑precision integers in the search queue.  The search time is
   O(7*n) and the memory usage is O(7*n). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* n is guaranteed to be ≥1 for valid calls */
    if (n == 0) return 0;

    /* total number of states */
    size_t total = 7 * n;

    /* parent array: -1 = unvisited, -2 = root (first digit) */
    int *parent = (int *)malloc(total * sizeof(int));
    if (!parent) return 0;
    memset(parent, 0xFF, total * sizeof(int));  /* set to -1 */

    /* digit array holds the digit appended to reach the state */
    int *digit = (int *)malloc(total * sizeof(int));
    if (!digit) { free(parent); return 0; }

    /* queue for BFS: store state indices */
    int *queue = (int *)malloc(total * sizeof(int));
    if (!queue) { free(parent); free(digit); return 0; }

    size_t head = 0, tail = 0;
    int n_int = (int)n;          /* local copy for modulo ops */

    /* seed the queue with 1‑digit numbers (non‑zero) whose digit sum ≤6 */
    for (int d = 1; d <= 9 && d <= 6; ++d) {
        int rem = d % n_int;
        int sum = d;
        size_t idx = (size_t)sum * n + rem;
        parent[idx] = -2;              /* root sentinel */
        digit[idx] = d;
        queue[tail++] = (int)idx;
    }

    /* BFS */
    while (head < tail) {
        int curIdx = queue[head++];
        int sum = curIdx / n_int;
        int rem = curIdx % n_int;

        /* Found solution */
        if (sum == 6 && rem == 0) {
            /* Reconstruct the number from the path */
            /* maximum possible length is total states (worst case) */
            int *rev_digits = (int *)malloc(total * sizeof(int));
            if (!rev_digits) {
                free(parent);
                free(digit);
                free(queue);
                return 0;
            }
            size_t len = 0;
            int idx = curIdx;
            while (parent[idx] != -2) {
                rev_digits[len++] = digit[idx];
                idx = parent[idx];
            }
            /* include the root digit */
            rev_digits[len++] = digit[idx];

            /* Build the result from most significant to least */
            seq_t result = 0;
            for (size_t i = 0; i < len; ++i) {
                result = result * 10 + rev_digits[len - 1 - i];
            }

            free(rev_digits);
            free(parent);
            free(digit);
            free(queue);
            return result;
        }

        /* Expand children by appending digits 0–9 (subject to sum ≤6) */
        for (int d = 0; d <= 9; ++d) {
            int newSum = sum + d;
            if (newSum > 6) continue;
            int newRem = (rem * 10 + d) % n_int;
            size_t newIdx = (size_t)newSum * n + newRem;
            if (parent[newIdx] != -1) continue;  /* already visited */
            parent[newIdx] = curIdx;
            digit[newIdx] = d;
            queue[tail++] = (int)newIdx;
        }
    }

    /* No multiple found */
    free(parent);
    free(digit);
    free(queue);
    return 0;
}
