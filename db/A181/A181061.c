/*
 * A181061  a(n) is the smallest positive number such that the decimal digits
 *          of n*a(n) are all 0, 1 or 2.
 * https://oeis.org/A181061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 998, u64 n <= 9375,
 *            u128 n <= 9375, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Find the smallest positive k such that the decimal digits of n*k belong
 * to {0,1,2}.  This is done by BFS over remainders of a multiple consisting
 * solely of digits 0,1,2.  The minimal multiple m is reconstructed and
 * returned as k = m / n. */
seq_t a(idx_t n)
{
    /* Special offset: a(0) = 1 */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    uint32_t N = (uint32_t)n;
    /* Allocate arrays: parent[rem] is the previous remainder,
     * digit[rem] is the digit appended to reach rem. */
    uint32_t *parent = (uint32_t*)malloc(sizeof(uint32_t) * N);
    uint8_t  *digit  = (uint8_t*)malloc(sizeof(uint8_t)  * N);
    uint32_t *queue  = (uint32_t*)malloc(sizeof(uint32_t) * N);
    const uint32_t UNVIS = UINT32_MAX;
    for (uint32_t i = 0; i < N; ++i) {
        parent[i] = UNVIS;
    }

    uint32_t head = 0, tail = 0;
    bool found = false;

    /* Start with first digits 1 and 2 (leading zero is not allowed). */
    for (int d = 1; d <= 2; ++d) {
        uint32_t r = (uint32_t)(d % N);
        if (parent[r] == UNVIS) {
            parent[r] = r;          /* root marks itself as parent */
            digit[r]  = (uint8_t)d;
            queue[tail++] = r;
            if (r == 0) {
                found = true;
                break;
            }
        }
    }

    /* BFS over remainders */
    while (!found && head < tail) {
        uint32_t cur = queue[head++];
        for (int d = 0; d <= 2; ++d) {
            uint32_t nr = ((cur * 10u) + (uint32_t)d) % N;
            if (parent[nr] == UNVIS) {
                parent[nr] = cur;
                digit[nr]  = (uint8_t)d;
                queue[tail++] = nr;
                if (nr == 0) {
                    found = true;
                    break;
                }
            }
        }
    }

    /* Reconstruct the minimal multiple m (digits only 0/1/2) */
    uint8_t *rev = (uint8_t*)malloc(sizeof(uint8_t) * (N + 1));
    uint32_t rev_len = 0;
    uint32_t cur = 0;
    while (1) {
        rev[rev_len++] = digit[cur];
        if (parent[cur] == cur) break;   /* reached the root digit */
        cur = parent[cur];
    }

    /* Assemble the number from most significant digit to least */
    seq_t m = (seq_t)0;
    for (int i = (int)rev_len - 1; i >= 0; --i) {
        m = m * (seq_t)10 + (seq_t)rev[i];
    }

    /* Clean up */
    free(rev);
    free(queue);
    free(digit);
    free(parent);

    /* The desired a(n) is the quotient m / n */
    return m / (seq_t)n;
}
