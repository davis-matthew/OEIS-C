/*
 * A086319  Primes produced by repeated application of the formula p -> (4p +-
 *          3) starting at the prime 2.
 * https://oeis.org/A086319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 22, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A086319:
   primes produced by repeated application of the formula p -> (4p ± 3)
   starting at the prime 2.  The code performs a breadth‑first traversal
   of the tree defined by the two operators, memoises the result, and
   returns the nth element (n≥1). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* External linkage, exact signature required by the platform. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* For safety, though the OEIS offset is 1; callers will not use n==0. */
        return 0;
    }

    /* Static memoisation state: queue of primes and
       indices for breadth–first generation. */
    static seq_t *queue = NULL;
    static idx_t capacity = 0;
    static idx_t size = 0;          /* number of stored primes */
    static idx_t head = 0;          /* next prime to process */

    /* Initialise on first call. */
    if (queue == NULL) {
        capacity = 8;
        queue    = seq_alloc(capacity);
        queue[0] = 2;   /* a(1) = 2 */
        size = 1;
    }

    /* Grow the sequence until we have at least n terms. */
    while (size < n) {
        seq_t p = queue[head++];

        /* 4p - 3 */
        seq_t child1 = (seq_t)4 * p - 3;
        if (is_prime_big(child1)) {
            if (size == capacity) {
                idx_t oldcap = capacity;
                capacity *= 2;
                queue = seq_realloc(queue, oldcap, capacity);
            }
            queue[size++] = child1;
        }

        /* 4p + 3 */
        seq_t child2 = (seq_t)4 * p + 3;
        if (is_prime_big(child2)) {
            if (size == capacity) {
                idx_t oldcap = capacity;
                capacity *= 2;
                queue = seq_realloc(queue, oldcap, capacity);
            }
            queue[size++] = child2;
        }
    }

    return queue[n - 1];
}
