/*
 * A050823  Even numbers in Hofstadter Q-sequence
 *          a(n)=a(n-a(n-1))+a(n-a(n-2)).
 * https://oeis.org/A050823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th even value of the Hofstadter Q‑sequence defined
 *            by: Q(1)=Q(2)=1, Q(k)=Q(k‑Q(k‑1))+Q(k‑Q(k‑2)) for k>2. The
 *            sequence a is formed by collecting the even Q(k), in the order
 *            of increasing k. a(1) is the first even value, a(2) the second,
 *            etc
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static tables for Q(k) and the list of even Q(k)s */
    static seq_t *q      = NULL; /* 1‑based index: q[1] = 1 */
    static size_t q_size = 0;    /* current allocated size of q */
    static size_t q_cap  = 0;    /* current capacity of q */

    static seq_t *evens      = NULL; /* list of even Q(k) values */
    static size_t evens_size = 0;    /* number of collected evens */
    static size_t evens_cap  = 0;    /* capacity of evens array */

    static idx_t next_k = 1;        /* next k to compute */

    /* Helper: grow an array when necessary */
    if (q_cap == 0) { /* first allocation */
        q_cap = 64;
        q = mem_new(seq_t, q_cap);
    }
    if (evens_cap == 0) {
        evens_cap = 64;
        evens = mem_new(seq_t, evens_cap);
    }

    /* Ensure we have computed enough even values to answer request */
    while (evens_size < n) {

        /* Extend q array if needed */
        if (next_k >= q_cap) {
            size_t new_cap = q_cap * 2;
            seq_t *tmp = mem_resize(seq_t, q, new_cap);
            q = tmp;
            q_cap = new_cap;
        }

        /* Compute Q(next_k)  */
        if (next_k == 1 || next_k == 2) {
            q[next_k] = 1;
        } else {
            /* indices for recursion */ 
            idx_t idx1 = (idx_t)q[next_k - 1];
            idx_t idx2 = (idx_t)q[next_k - 2];
            seq_t val1 = q[next_k - idx1];
            seq_t val2 = q[next_k - idx2];
            q[next_k] = val1 + val2;
        }

        /* If it's even, record it */
        if (((q[next_k] & 1) == 0)) {
            if (evens_size >= evens_cap) {
                size_t new_cap = evens_cap * 2;
                seq_t *tmp = mem_resize(seq_t, evens, new_cap);
                evens = tmp;
                evens_cap = new_cap;
            }
            evens[evens_size++] = q[next_k];
        }

        ++next_k;
    }

    /* Return the requested term (a(n) is 1‑based) */
    return evens[n - 1];
}
