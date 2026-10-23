/*
 * A161543  The m-th composite number composite(m) is a term iff composite(m)
 *          + m is composite.
 * https://oeis.org/A161543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Sequence A161543 – composite numbers whose index m satisfies
 *  composite(m)+m is composite.
 *
 *  The algorithm iterates through the positive integers, skips primes,
 *  counts the composite indices, and stores a composite if the sum with
 *  its index is also composite.  The result is memoised across calls.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Helper: deterministic Miller‑Rabin for 64‑bit values (tuned for speed). */
static inline bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *seq = NULL;   /* array of sequence values     */
    static size_t sz  = 0;      /* number of values stored     */
    static size_t cap = 0;      /* current capacity of seq     */

    /* state for incremental search */
    static uint64_t cur_num  = 4;   /* candidate number to test */
    static idx_t    comp_idx = 0;   /* current composite index (m) */

    /* if we already have the requested term, return it directly */
    if (sz >= n)
        return seq[n - 1];

    /* initialise the array on first call */
    if (cap == 0) {
        cap = 32;
        seq = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
    }

    /* generate new terms until we have at least n of them */
    while (sz < n) {
        /* advance cur_num until it is composite */
        while (is_prime_u64(cur_num))
            ++cur_num;

        /* cur_num is now a composite number, its index is comp_idx+1 */
        ++comp_idx;
        seq_t composite = (seq_t)cur_num;
        seq_t sum = composite + (seq_t)comp_idx;   /* composite + m */

        /* sum will be composite iff it is not prime */
        if (!is_prime_u64((uint64_t)sum)) {
            if (sz == cap) {        /* grow the array */
                cap <<= 1;
                seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
            }
            seq[sz++] = composite;
        }

        ++cur_num;   /* next integer to check */
    }

    return seq[n - 1];
}
