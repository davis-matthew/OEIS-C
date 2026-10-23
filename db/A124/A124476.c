/*
 * A124476  Smallest multiple of A047201(n) (i.e., numbers not divisible by 5)
 *          with only digits 6 and 7.
 * https://oeis.org/A124476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implements OEIS A124476:
   a(n) = smallest multiple of A047201(n) whose decimal representation
   uses only the digits 6 and 7.  
   A047201(n) = n + floor((n-1)/4)  (numbers not divisible by 5).
   The result is found by a BFS over states (remainder modulo m, used‑digit mask).
*/

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* m = A047201(n) */
    idx_t m = n + (n - 1) / 4;
    if (m == 0) return 0;   /* not needed for n>=1 */

    /* Number of states: 4 remainders * 4 masks */
    size_t maxStates = 4 * (size_t)m;

    /* Visited flag, predecessor index, digit used to reach state */
    uint8_t *visited = (uint8_t *)calloc(maxStates, sizeof(uint8_t));
    if (!visited) return 0; /* allocation failure */

    size_t *prev    = (size_t *)malloc(maxStates * sizeof(size_t));
    uint8_t *digit  = (uint8_t *)malloc(maxStates * sizeof(uint8_t));
    if (!prev || !digit) {
        free(visited);
        return 0;
    }

    /* BFS queue */
    size_t *queue = (size_t *)malloc(maxStates * sizeof(size_t));
    if (!queue) {
        free(visited);
        free(prev);
        free(digit);
        return 0;
    }
    size_t qhead = 0, qtail = 0;

    /* Helper to enqueue an initial state */
    for (uint8_t d = 6; d <= 7; ++d) {
        idx_t rem = d % m;
        size_t state = (size_t)rem * 4 + ((d == 6) ? 1 : 2);
        if (!visited[state]) {
            visited[state] = 1;
            prev[state] = SIZE_MAX;          /* no predecessor */
            digit[state] = d;
            queue[qtail++] = state;
        }
    }

    /* BFS */
    size_t goal = SIZE_MAX;
    while (qhead < qtail && goal == SIZE_MAX) {
        size_t cur = queue[qhead++];
        idx_t curRem = (idx_t)(cur / 4);
        uint8_t curMask = (uint8_t)(cur & 3);

        if (curRem == 0 && curMask == 3) {
            goal = cur;
            break;
        }

        for (uint8_t d = 6; d <= 7; ++d) {
            idx_t newRem = (curRem * 10 + d) % m;
            uint8_t newMask = curMask | ((d == 6) ? 1 : 2);
            size_t newState = (size_t)newRem * 4 + newMask;
            if (!visited[newState]) {
                visited[newState] = 1;
                prev[newState] = cur;
                digit[newState] = d;
                queue[qtail++] = newState;
            }
        }
    }

    /* No solution found (should never happen for m not divisible by 5) */
    if (goal == SIZE_MAX) {
        free(visited);
        free(prev);
        free(digit);
        free(queue);
        return 0;
    }

    /* Reconstruct the digit sequence (reverse order). */
    uint8_t *revDigits = (uint8_t *)malloc(maxStates * sizeof(uint8_t));
    if (!revDigits) {
        free(visited);
        free(prev);
        free(digit);
        free(queue);
        return 0;
    }
    size_t len = 0;
    size_t st = goal;
    while (1) {
        revDigits[len++] = digit[st];
        if (prev[st] == SIZE_MAX)
            break;
        st = prev[st];
    }

    /* Reverse to obtain the correct order. */
    for (size_t i = 0; i < len / 2; ++i) {
        uint8_t temp = revDigits[i];
        revDigits[i] = revDigits[len - 1 - i];
        revDigits[len - 1 - i] = temp;
    }

    /* Build the numeric value in seq_t. */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        result = result * 10 + (seq_t)revDigits[i];
    }

    /* Clean up */
    free(visited);
    free(prev);
    free(digit);
    free(queue);
    free(revDigits);

    return result;
}
