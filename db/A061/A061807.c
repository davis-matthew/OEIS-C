/*
 * A061807  Smallest positive multiple of n containing only even digits.
 * https://oeis.org/A061807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 98, u32 n <= 9998, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The smallest positive multiple of n that contains only even
   decimal digits (0,2,4,6,8) is computed by a breadth‑first
   search on remainders modulo n.  Each remainder stores the
   parent remainder and the digit that produced it, allowing the
   final number to be reconstructed without storing the large
   intermediate values. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for 0, return 0 */

    const uint8_t first_digits[4] = {2, 4, 6, 8};
    const uint8_t all_digits[5]   = {0, 2, 4, 6, 8};

    /* Number of possible remainders modulo n */
    const size_t N = (size_t)n;

    /* Allocate state arrays. Cast the void* returned by calloc/malloc
       so the code compiles in C++ as well. */
    bool *seen   = (bool *)calloc(N, sizeof(bool));
    idx_t *parent = (idx_t *)malloc(N * sizeof(idx_t));
    uint8_t *digit = (uint8_t *)malloc(N * sizeof(uint8_t));
    idx_t *queue = (idx_t *)malloc(N * sizeof(idx_t));

    if (!seen || !parent || !digit || !queue) {
        free(seen);
        free(parent);
        free(digit);
        free(queue);
        return 0; /* allocation failure */
    }

    size_t head = 0, tail = 0;
    bool found_zero = false;
    idx_t target_rem = 0;

    /* Initialise the BFS with one‑digit numbers (non‑zero). */
    for (int i = 0; i < 4; ++i) {
        uint8_t d = first_digits[i];
        idx_t rem = (idx_t)(d % n);
        if (!seen[rem]) {
            seen[rem] = true;
            parent[rem] = n;      /* sentinel for root */
            digit[rem]  = d;
            queue[tail++] = rem;
            if (rem == 0) {
                seq_t res = (seq_t)d;
                free(seen);
                free(parent);
                free(digit);
                free(queue);
                return res;        /* single digit already divisible */
            }
        }
    }

    /* Breadth‑first exploration of remainders. */
    while (head < tail && !found_zero) {
        idx_t cur = queue[head++];
        for (int i = 0; i < 5; ++i) {
            uint8_t d = all_digits[i];
            idx_t rem = (cur * 10 + d) % n;
            if (!seen[rem]) {
                seen[rem] = true;
                parent[rem] = cur;
                digit[rem]  = d;
                queue[tail++] = rem;
                if (rem == 0) {
                    target_rem = rem;
                    found_zero = true;
                    break;
                }
            }
        }
    }

    /* Reconstruct the number from remainder 0 back to the start. */
    uint8_t *stack = (uint8_t *)malloc(N * sizeof(uint8_t));
    size_t len = 0;
    idx_t rem = target_rem;
    while (true) {
        stack[len++] = digit[rem];
        if (parent[rem] == n) break;   /* reached a root node */
        rem = parent[rem];
    }

    seq_t result = 0;
    for (size_t i = len; i > 0; --i) {
        result = result * 10 + stack[i - 1];
    }

    free(stack);
    free(seen);
    free(parent);
    free(digit);
    free(queue);

    return result;
}
