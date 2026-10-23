/*
 * A105136  Numbers n such that 64n+33 is prime.
 * https://oeis.org/A105136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth integer n such that 64*n + 33 is prime
 * Range:     u8 n <= 58, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* declares bool is_prime(uint64_t) */

seq_t a(idx_t n)
{
    /* Static buffer to cache generated terms and progress */
    static seq_t *buffer      = NULL;   /* array of found n's */
    static size_t  capacity   = 0;      /* allocated size of buffer   */
    static idx_t   stored     = 0;      /* number of values already in buffer */
    static idx_t   last_checked = 0;    /* last candidate examined    */

    /* Offset 1: for safety return 0 for requested index 0 */
    if (n == 0)
        return (seq_t)0;

    /* Return cached value if available */
    if (n <= stored)
        return buffer[n - 1];

    /* Allocate initial buffer if needed */
    if (!buffer) {
        capacity = 128;
        buffer = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!buffer)
            return (seq_t)0;   /* allocation failure */
    }

    /* Generate terms until we have at least n of them */
    idx_t cand = last_checked + 1;
    while (stored < n) {
        /* Compute 64*cand + 33, test for primality */
        uint64_t candidate_value = 64ULL * (uint64_t)cand + 33ULL;
        if (is_prime(candidate_value)) {
            /* Grow buffer if necessary */
            if (stored == capacity) {
                capacity <<= 1;
                seq_t *tmp = (seq_t *)realloc(buffer, capacity * sizeof(seq_t));
                if (!tmp)
                    return (seq_t)0;
                buffer = tmp;
            }
            buffer[stored++] = (seq_t)cand;
        }
        last_checked = cand;
        cand++;
    }
    return buffer[n - 1];
}
