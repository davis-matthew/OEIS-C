/*
 * A194416  Numbers m such that Sum_{k=1..m} (<1/3 + k*r> - <k*r>) = 0, where
 *          r=sqrt(3) and < > denotes fractional part.
 * https://oeis.org/A194416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m such that Sum_{k=1..m} ( {1/3+k*sqrt(3)} - {k*sqrt(3)}
 *            ) = 0, where {x} denotes the fractional part of x
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Integer square root for 64‑bit values.  Works for all arguments
   that will be provided (up to about 3·(10 000)²). */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = (n >> 1) + 1;   /* high is guaranteed > sqrt(n) */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;  /* upper median to avoid infinite loop */
        if (mid > 0 && mid > n / mid)   /* mid*mid > n, guard against overflow */
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Cache of already computed terms. */
    static seq_t *terms = NULL;
    static size_t term_count = 0;    /* number of cached terms */
    static size_t capacity = 0;      /* allocated size of 'terms' */

    /* State for incremental construction of the sequence. */
    static uint64_t last_m = 0;      /* largest m processed so far */
    static uint64_t last_A = 0;      /* number of k ≤ last_m with
                                       {k*sqrt3} < 2/3 */

    /* Grow the cache if necessary. */
    if (term_count + 1 > capacity) {
        size_t new_cap = capacity > 0 ? capacity << 1 : 8;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* Incrementally search for further terms until we have at least n of them. */
    while (term_count < (size_t)n) {
        ++last_m;
        uint64_t m = last_m;
        uint64_t k = m;

        /* Compute floor(k*sqrt(3))  and  floor(3*k*sqrt(3)). */
        uint64_t a_k = isqrt_u64(3 * k * k);   /* floor(k*sqrt(3))   */
        uint64_t c_k = isqrt_u64(27 * k * k);  /* floor(3*k*sqrt(3)) */

        /* Condition {k*sqrt3} < 2/3  ⇔  c_k - 3*a_k <= 1 */
        if (c_k - 3 * a_k <= 1) {
            ++last_A;
        }

        if (m % 3 == 0) {   /* only multiples of 3 can be solutions */
            if (last_A == (2 * m) / 3) {
                terms[term_count++] = (seq_t)m;
            }
        }
    }

    return terms[(size_t)n - 1];
}
