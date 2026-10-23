/*
 * A036762  The integer values of x/d(x) in order of magnitude of x in
 *          A033950.
 * https://oeis.org/A036762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 4095, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer quotients of n/τ(n) for n where n/τ(n) is integer, sorted by n. */
#include <stdlib.h>

static size_t term_count = 0;
static idx_t next_x = 1;
static seq_t *terms = NULL;
static size_t cache_capacity = 0;

/* Simple divisor count using trial division. */
static uint64_t divisor_count(uint64_t n) {
    uint64_t cnt = 1;
    uint64_t e = 0;
    while ((n & 1) == 0) {
        n >>= 1;
        ++e;
    }
    if (e) cnt *= (e + 1);

    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            cnt *= (e + 1);
        }
    }
    if (n > 1) cnt *= 2;
    return cnt;
}

/* Ensure the cache can hold at least 'needed' terms. */
static void ensure_capacity(size_t needed) {
    if (needed <= cache_capacity) return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *newp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!newp) { /* Allocation failed: abort to avoid UB. */
        if (terms) free(terms);
        exit(1);
    }
    terms = newp;
    cache_capacity = new_cap;
}

/* Compute the n-th term of the sequence. */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* Undefined, but safe return. */
    while (term_count < n) {
        uint64_t d = divisor_count((uint64_t)next_x);
        if ((uint64_t)next_x % d == 0) {
            seq_t q = (seq_t)next_x / (seq_t)d;
            ensure_capacity(term_count + 1);
            terms[term_count++] = q;
        }
        ++next_x;
    }
    return terms[n - 1];
}
