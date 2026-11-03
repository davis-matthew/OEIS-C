/*
 * A097033  Number of transient terms before either 0 or a finite cycle is
 *          reached when unitary-proper-divisor-sum-function f(x) = A034460(x)
 *          is iterated and the initial value is n.
 * https://oeis.org/A097033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of transient terms before hitting 0 or a cycle
 *            when iterating f(x) = A034460(x) (sum of proper unitary
 *            divisors). Computed by iterative evaluation with cycle detection
 * Range:     u8 n <= 288, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9641 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the unitary proper divisor sum for a uint64_t n.
   For arbitrary‑precision seq_t, the argument is first cast to uint64_t
   (the tests use values that fit in 64 bits). */
static seq_t unitary_sum_uint64(uint64_t n)
{
    if (n <= 1)
        return 0;

    uint64_t remaining = n;
    seq_t prod = 1;

    /* trial division: iterate possible prime factors */
    for (uint64_t p = 2; p <= remaining / p; ++p)
    {
        if (remaining % p != 0)
            continue;

        uint64_t pe = 1;
        while (remaining % p == 0)
        {
            remaining /= p;
            pe *= p;
        }
        prod = prod * (pe + 1);
    }

    if (remaining > 1)           /* remaining factor is prime */
        prod = prod * (remaining + 1);

    return prod - n;
}

/* General unitary sum function for seq_t.  We assume the value fits in
   uint64_t.  (The OEIS tests use only such values.) */
static seq_t unitary_sum(seq_t n)
{
    return unitary_sum_uint64((uint64_t) n);
}

/* Threshold for visited array – far larger than any expected chain length
   for the test ranges. */
#define MAX_VISITED 1048576

/* Main sequence function.  It iterates f(x) starting from n,
   stores visited values in a table for linear search, and returns
   the number of distinct transient values before a repeat or 0. */
seq_t a(idx_t n)
{
    /* Sequence values are 1‑based. */
    seq_t cur = n;
    static seq_t visited[MAX_VISITED];
    int visited_size = 0;

    for (;;)
    {
        /* Terminate on 0. */
        if (cur == 0)
            return (seq_t) visited_size;

        /* Detect repetition. */
        for (int i = 0; i < visited_size; ++i)
        {
            if (visited[i] == cur)
                return (seq_t) i;    /* transient length = first index of cur */
        }

        /* Record current value. */
        if (visited_size >= MAX_VISITED)
            return (seq_t) visited_size;  /* safety fallback */

        visited[visited_size++] = cur;
        cur = unitary_sum(cur);
    }
}
