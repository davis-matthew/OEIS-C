/*
 * A104788  Numbers q such that q^2 = 2*prime(n) + n for some n.
 * https://oeis.org/A104788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt(2⋅prime(k)+k) where k is the index of the prime in
 *            the list; the term a(n) is the nth such square‑root value,
 *            listed in increasing order. The sequence is computed by scanning
 *            successive prime indices, testing 2⋅prime(k)+k for a perfect
 *            square, and collecting the square roots
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 33, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_square, isqrt, miller_rabin, push_seq, seq_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/miller_rabin.h"
#include "../utils/is_square.h"
#include "../utils/isqrt.h"
#include "../utils/push_seq.h"
#include "../utils/seq_malloc.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, behaviour for n<1 is undefined */

    /* Static storage for the sequence values (q's) and state for
       generating further terms. */
    static seq_t *seq = NULL;
    static idx_t size = 0;
    static idx_t cap  = 0;

    static uint64_t next_index = 1;   /* next prime index to process (1‑based) */
    static uint64_t candidate  = 2;   /* next integer to test for primality */

    /* Initialise the dynamic array on the first call. */
    if (!seq) {
        cap  = 64;
        seq  = seq_malloc(cap);
        size = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (size < n) {
        uint64_t p;   /* value of the prime at `next_index` */

        /* Find the next prime.  Each integer is tested only once, so the
           overall work is linear in the searched range. */
        while (true) {
            if (miller_rabin(candidate)) {
                p = candidate;
                candidate++;          /* start next search one past this prime */
                break;
            }
            candidate++;
        }

        /* Compute RHS = 2*prime + index.  Use seq_t for all arithmetic
           to avoid overflow on the target build. */
        seq_t rhs = (seq_t)2 * (seq_t)p + (seq_t)next_index;

        if (is_square(rhs)) {
            seq_t q = isqrt(rhs);
            push_seq(&seq, &cap, &size, q);
        }

        /* Move to the next prime index. */
        next_index++;
    }

    /* Return the requested term (1‑based indexing). */
    return seq[n - 1];
}
