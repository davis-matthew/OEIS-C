/*
 * A087657  a(n) = abs(prime(n)-a(n-1)) + abs(prime(n)-a(n-2)) +
 *          abs(prime(n)-a(n-3)).
 * https://oeis.org/A087657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = abs(prime(n)-a(n-1)) + abs(prime(n)-a(n-2)) +
 *            abs(prime(n)-a(n-3))
 * Range:     u8 n <= 54, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   get_prime, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Offset: 1 (a(1) = 1, a(2) = 1, a(3) = 12) */

#include <stddef.h>                       /* for size_t */
#include "../utils/get_prime.h"
#include "../utils/grow_seq.h"

static seq_t *a_cache = NULL;   /* cache for a(1..k) values (index 0 unused) */
static size_t known = 0;        /* largest index computed so far (>= 0) */
static size_t cap = 0;          /* current allocated capacity of a_cache */

seq_t a(idx_t n) {
    /* Undefined behaviour for n==0; return 0 to avoid accessing cache[-1] */
    if (n == 0) return 0;

    /* If already computed, return the cached value */
    if ((size_t)n <= known) return a_cache[(size_t)n];

    /* Expand cache to hold up to n (index 0 unused) */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        grow_seq(&a_cache, &cap, need);
    }

    /* Compute new elements from known+1 up to n */
    for (size_t i = known + 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            a_cache[i] = 1;
        } else if (i == 2) {
            a_cache[i] = 1;
        } else if (i == 3) {
            a_cache[i] = 12;
        } else {
            /* Prime at position i */
            uint32_t p_u32 = get_prime((idx_t)i);
            seq_t p = (seq_t)p_u32;

            /* Previous three terms */
            seq_t a1 = a_cache[i - 1];
            seq_t a2 = a_cache[i - 2];
            seq_t a3 = a_cache[i - 3];

            /* Absolute differences (prevent unsigned underflow) */
            seq_t d1 = (p > a1) ? (p - a1) : (a1 - p);
            seq_t d2 = (p > a2) ? (p - a2) : (a2 - p);
            seq_t d3 = (p > a3) ? (p - a3) : (a3 - p);

            a_cache[i] = d1 + d2 + d3;
        }
    }

    known = (size_t)n;
    return a_cache[(size_t)n];
}
