/*
 * A086319  Primes produced by repeated application of the formula p -> (4p +-
 *          3) starting at the prime 2.
 * https://oeis.org/A086319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: full, pass, 25 terms through n = 25 (2026-11-03)
 * Helpers:   is_prime_u64
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
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

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
