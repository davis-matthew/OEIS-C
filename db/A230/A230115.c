/*
 * A230115  Numbers n such that tau(n+1) - tau(n) = 2; where tau(n) = the
 *          number of divisors of n (A000005).
 * https://oeis.org/A230115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m such that τ(m+1) - τ(m) = 2 (A230115)
 * Range:     u8 n <= 23, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/divisor_count_u64.h"

/* --------- Cache handling --------------------------------------------- */
static seq_t *cached = NULL;      /* dynamic array of found terms           */
static size_t cached_len = 0;     /* number of terms stored                */
static size_t cached_cap = 0;     /* current capacity of the array         */
static idx_t next_scan = 1;       /* next integer to test                  */

/* Grow the cache so that it can hold at least one more element. */
static void grow_cache(void)
{
    size_t new_cap = (cached_cap == 0) ? 16 : cached_cap * 2;
    /* realloc is required to be cast in C++ */
    seq_t *tmp = (seq_t *)realloc(cached, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* unrecoverable memory exhaustion       */
    cached = tmp;
    cached_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* a(idx_t n) : returns the n-th element of the sequence (n ≥ 1).  If n=0
 * the result is 0 (undefined for the sequence).  The function builds
 * a simple incremental cache, so any order of calls is supported.     */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Expand cache until we have at least n terms. */
    while (cached_len < n) {
        uint64_t x = (uint64_t)next_scan++;          /* test integer   */

        uint64_t tau_x   = divisor_count_u64(x);
        uint64_t tau_xp1 = divisor_count_u64(x + 1);

        if (tau_xp1 == tau_x + 2) {                 /* τ(x+1)-τ(x)=2  */
            if (cached_len == cached_cap)
                grow_cache();
            cached[cached_len++] = (seq_t)x;
        }
    }
    return cached[n - 1];
}
