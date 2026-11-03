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
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   is_prime_u64, is_square, isqrt_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/is_square.h"
#include "../utils/isqrt_seq.h"
#include "../utils/mem.h"

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

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
            if (is_prime_u64(candidate)) {
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
            seq_t q = isqrt_seq(rhs);
            push_seq(&seq, &cap, &size, q);
        }

        /* Move to the next prime index. */
        next_index++;
    }

    /* Return the requested term (1‑based indexing). */
    return seq[n - 1];
}
