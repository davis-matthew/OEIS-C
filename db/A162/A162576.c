/*
 * A162576  Primes of the form x^2+2^x+y^2+2^y, with x and y nonnegative.
 * https://oeis.org/A162576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   prime numbers of the form x^2 + 2^x + y^2 + 2^y with x and y
 *            positive integers (x ≥ 1, y ≥ 1), listed in increasing order.
 *            a(n) is the nth such prime (a(1)=11)
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 17, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   compare_seq_t, ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation incrementally generates all unordered pairs
   (x,y) with x ≤ y.  For each new maximum coordinate y = L,
   it evaluates all pairs (x, L) with 1 ≤ x ≤ L, tests
   primality, and inserts the resulting prime into a static
   sorted array.  The array expands only as far as needed to
   answer the largest n requested. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_big.h"
#include "../utils/compare_seq_t.h"
#include "../utils/ensure_capacity.h"

/* Insert x into a sorted array arr[0..cnt-1] if not already present.
   arr may be reallocated by ensure_capacity, and *arrp updated. */
static void
insert_sorted(seq_t **arrp, size_t *cnt, size_t *cap, seq_t x)
{
    seq_t *arr = *arrp;
    size_t lo = 0, hi = *cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        int cmp = compare_seq_t(&arr[mid], &x);
        if (cmp < 0)
            lo = mid + 1;
        else if (cmp > 0)
            hi = mid;
        else
            return;                 /* duplicate */
    }
    ensure_capacity(*cnt + 1, cap, (void **)arrp, sizeof(seq_t));
    arr = *arrp;                      /* arr may have moved after realloc */
    memmove(arr + lo + 1, arr + lo, (*cnt - lo) * sizeof(seq_t));
    arr[lo] = x;
    (*cnt)++;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but harmless */

    /* static state ----------------------------------------------------------
     *   primes:     sorted list of all primes found so far
     *   primes_cnt: number of primes stored
     *   primes_cap: capacity of primes array
     *   limit:      current maximum value of x and y seen
     *   pow2:       array of powers of two, pow2[i] = 2^i
     *   pow2_cap:   capacity of pow2 array
     *   pow2_cnt:   number of entries in pow2 (equals limit + 1)
     */
    static seq_t *primes = NULL;
    static size_t primes_cnt = 0, primes_cap = 0;
    static seq_t *pow2 = NULL;
    static size_t pow2_cap = 0, pow2_cnt = 0;
    static idx_t limit = 0;

    /* initialize pow2[0] */
    if (pow2_cnt == 0) {
        ensure_capacity(1, &pow2_cap, (void **)&pow2, sizeof(seq_t));
        pow2[0] = (seq_t)1;
        pow2_cnt = 1;
    }

    /* generate new candidates until we have at least n primes */
    while (primes_cnt < (size_t)n) {
        limit++;                         /* new y = limit */

        /* extend pow2 array if necessary */
        if ((size_t)limit >= pow2_cap)
            ensure_capacity(limit + 1, &pow2_cap, (void **)&pow2, sizeof(seq_t));

        /* compute 2^limit from previous value */
        pow2[limit] = pow2[limit - 1] * (seq_t)2;
        pow2_cnt = limit + 1;

        seq_t pow_limit = pow2[limit];
        seq_t limit_sq = (seq_t)limit * (seq_t)limit;

        /* consider all pairs (x, limit) with 1 ≤ x ≤ limit */
        for (idx_t x = 1; x <= limit; ++x) {
            seq_t pow_x = pow2[x];
            seq_t val = (seq_t)x * (seq_t)x + pow_x + limit_sq + pow_limit;
            if (is_prime_big(val))
                insert_sorted(&primes, &primes_cnt, &primes_cap, val);
        }
    }

    return primes[n - 1];
}
