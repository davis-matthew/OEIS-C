/*
 * A190612  Numbers k such that (tau(k-1) + tau(k+1))/tau(k) is an integer,
 *          where tau(k)=A000005(k).
 * https://oeis.org/A190612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A190612: numbers k such that (tau(k-1)+tau(k+1))/tau(k) is an integer. */
/* The implementation builds the sequence on demand by testing integers in order and
   storing the results so that subsequent calls can reuse previously computed terms. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Compute tau(n) – the number of divisors of n.  The function works for
   n up to the maximum value that fits in a uint64_t.  The result is held in
   type seq_t, which may be an unsigned integer type or an arbitrary‑precision
   class. */
static seq_t divisor_count(uint64_t n) {
    if (n <= 1)          /* tau(0) is undefined; tau(1) = 1 */
        return (seq_t)1;
    seq_t prod = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* multiply in (exp + 1) */
            prod *= (seq_t)(exp + 1);
        }
    }
    if (m > 1)           /* remaining prime factor */
        prod *= (seq_t)2;
    return prod;
}

/* Ensure the dynamic array has room for at least `need` elements. */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t need) {
    if (*cap >= need)
        return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap * 2);
    if (new_cap < need)
        new_cap = need;
    seq_t *new_arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *arr = new_arr;
    *cap = new_cap;
}

/* Return the n-th element of A190612 (1‑based indexing). */
seq_t a(idx_t n) {
    static seq_t *seq      = NULL;   /* array of found terms      */
    static size_t seq_size = 0;      /* number of terms stored    */
    static size_t seq_cap  = 0;      /* allocated capacity        */
    static idx_t  candidate = 2;     /* next integer to test      */

    /* Generate terms until we have at least n of them. */
    while (seq_size < n) {
        /* Find the next k that satisfies the defining condition. */
        for (; ; ++candidate) {
            uint64_t k = (uint64_t)candidate;
            seq_t t_km1 = divisor_count(k - 1);
            seq_t t_k   = divisor_count(k);
            seq_t t_kp1 = divisor_count(k + 1);
            if ((t_km1 + t_kp1) % t_k == 0) {
                /* Store the found term. */
                ensure_capacity(&seq, &seq_cap, seq_size + 1);
                seq[seq_size++] = (seq_t)k;
                /* Next candidate starts after the found k. */
                candidate = k + 1;
                break;
            }
        }
    }

    return seq[n - 1];
}
