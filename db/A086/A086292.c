/*
 * A086292  Number of divisors of 7-smooth numbers.
 * https://oeis.org/A086292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005( A002473(n) )
 * Range:     u8 n <= 1652, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code enumerates the 7‑smooth numbers (products of 2,3,5,7 only)
   via the classic multiple‑pointer method.  For each nth smooth number
   it computes its divisor count by factoring it over the same prime set.
   The routine is memoised so that calls with arbitrary n require no
   recomputation of previously generated smooth numbers. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- local helper: divisor count of a 7‑smooth 64‑bit value --- */
static seq_t divisor_count_u64(uint64_t x)
{
    seq_t count = 1;
    int e;

    e = 0;                          /* factor 2 */
    while ((x & 1ULL) == 0) { e++; x >>= 1; }
    count *= (seq_t)(e + 1);

    e = 0;                          /* factor 3 */
    while (x % 3ULL == 0) { e++; x /= 3ULL; }
    count *= (seq_t)(e + 1);

    e = 0;                          /* factor 5 */
    while (x % 5ULL == 0) { e++; x /= 5ULL; }
    count *= (seq_t)(e + 1);

    e = 0;                          /* factor 7 */
    while (x % 7ULL == 0) { e++; x /= 7ULL; }
    count *= (seq_t)(e + 1);

    return count;
}

seq_t a(idx_t n)
{
    /* Cache of 7‑smooth numbers and generation indices   */
    static uint64_t *smooth   = NULL;   /* 1‑based array of smooth numbers   */
    static idx_t    size     = 0;        /* number of entries stored          */
    static idx_t    capacity = 0;        /* allocated capacity (>= size)     */
    static idx_t    i2 = 1, i3 = 1, i5 = 1, i7 = 1;  /* pointers for next multiples */

    /* generate until we have at least n terms */
    if (size < n)
    {
        idx_t needed = n;

        /* first allocation */
        if (capacity == 0)
        {
            capacity = 16;
            smooth   = (uint64_t *)malloc((capacity + 1) * sizeof(uint64_t));
            smooth[1] = 1;                  /* 7‑smooth number #1 */
            size     = 1;
        }

        /* grow allocation if required */
        if (capacity < needed)
        {
            if (capacity < 64) capacity = 64;
            while (capacity < needed) capacity <<= 1;
            smooth = (uint64_t *)realloc(smooth, (capacity + 1) * sizeof(uint64_t));
        }

        /* produce the remaining smooth numbers */
        for (idx_t pos = size + 1; pos <= needed; ++pos)
        {
            uint64_t v2 = smooth[i2] * 2;
            uint64_t v3 = smooth[i3] * 3;
            uint64_t v5 = smooth[i5] * 5;
            uint64_t v7 = smooth[i7] * 7;

            uint64_t next = v2;
            if (v3 < next) next = v3;
            if (v5 < next) next = v5;
            if (v7 < next) next = v7;

            smooth[pos] = next;

            if (next == v2) ++i2;
            if (next == v3) ++i3;
            if (next == v5) ++i5;
            if (next == v7) ++i7;
        }

        size = needed;
    }

    /* compute divisor count of the required 7‑smooth number */
    uint64_t val = smooth[n];
    seq_t    tau = divisor_count_u64(val);
    return tau;
}
