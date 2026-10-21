/*
 * A069529  Smallest multiple of n with digit sum = 10, or 0 if no such number
 *          exists, e.g. a(3k)= 0.
 * https://oeis.org/A069529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the least positive multiple of n whose decimal digit sum
 *            is 10; a(n) = 0 if no such multiple exists
 * Range:     u8 n <= 19, u16 n <= 198, u32 n <= 4998, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence has offset 1 – ignore n == 0. */
    if (n == 0) return 0;

    /* Multiples of 3 cannot have digit sum 10 (10 mod 3 = 1). */
    if ((n % 3) == 0) return 0;

    const int SUM_MAX = 10;
    const size_t STATE_CNT = (size_t)n * (SUM_MAX + 1);

    /* Allocate state tables. */
    int   *parent  = (int   *)malloc(STATE_CNT * sizeof(int));
    int   *digit   = (int   *)malloc(STATE_CNT * sizeof(int));
    unsigned char *seen =
        (unsigned char *)malloc(STATE_CNT * sizeof(unsigned char));
    int *queue = (int *)malloc(STATE_CNT * sizeof(int));
    if (!parent || !digit || !seen || !queue) {
        /* Allocation failure – return 0 (safe fallback). */
        free(parent); free(digit); free(seen); free(queue);
        return 0;
    }
    for (size_t i = 0; i < STATE_CNT; ++i) {
        parent[i] = -1;
        seen[i]   = 0;
    }

    size_t qhead = 0, qtail = 0;

    /* Helper to compute state index. */
    #define IDX(rem, sum) ((size_t)(rem) * (SUM_MAX + 1) + (sum))

    /* Initialise states with the first digit 1…9. */
    for (int d = 1; d <= 9; ++d) {
        int rem = d % (int)n;
        int sum = d;
        if (sum > SUM_MAX) continue;
        size_t id = IDX(rem, sum);
        if (!seen[id]) {
            seen[id] = 1;
            digit[id]   = d;
            parent[id]  = -1;          /* start of a number */
            queue[qtail++] = (int)id;
            if (rem == 0 && sum == SUM_MAX) {
                /* Single‑digit solution. */
                free(parent); free(digit); free(seen); free(queue);
                return (seq_t)d;
            }
        }
    }

    /* Breadth‑first search over (remainder, digit sum). */
    while (qhead < qtail) {
        size_t cur = queue[qhead++];
        int cur_rem = (int)(cur / (SUM_MAX + 1));
        int cur_sum = (int)(cur % (SUM_MAX + 1));

        for (int d = 0; d <= 9; ++d) {
            int new_sum = cur_sum + d;
            if (new_sum > SUM_MAX) continue;

            int new_rem = (int)(((long long)cur_rem * 10 + d) % n);
            size_t id = IDX(new_rem, new_sum);

            if (!seen[id]) {
                seen[id] = 1;
                parent[id] = (int)cur;
                digit[id] = d;
                queue[qtail++] = (int)id;

                if (new_rem == 0 && new_sum == SUM_MAX) {
                    /* Reconstruct the number from the state chain. */
                    int revLen = 0;
                    int *revDigits = (int *)malloc(qtail * sizeof(int));
                    if (!revDigits) {
                        free(parent); free(digit); free(seen); free(queue);
                        return 0;
                    }
                    int state = (int)id;
                    while (state != -1) {
                        revDigits[revLen++] = digit[state];
                        state = parent[state];
                    }
                    /* Build seq_t value from most significant digit. */
                    seq_t value = 0;
                    for (int i = revLen - 1; i >= 0; --i) {
                        value = value * 10 + (seq_t)revDigits[i];
                    }
                    free(revDigits);
                    free(parent); free(digit); free(seen); free(queue);
                    return value;
                }
            }
        }
    }

    /* No multiple with digit sum 10 found. */
    free(parent); free(digit); free(seen); free(queue);
    return 0;
}
