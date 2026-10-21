/*
 * A082662  Numbers k such that the odd part of k is less than sqrt(2k).
 * https://oeis.org/A082662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A082662: a(n) is the n-th integer k such that the odd part of k
   (i.e., k divided by its largest power of two) satisfies
   odd(k) < sqrt(2 k).  The function enumerates successive k, tests the
   inequality by integer arithmetic, and returns the k that reaches the
   desired count.  */
/* This implementation uses only integer operations and works for any
   seq_t/idx_t pair provided by the build system.  */

#include <stdint.h>   /* ensures definition of uint64_t for intermediate ops */

/* Sequence element a(n), n ≥ 1.  a(1) is the first term of the OEIS list.  */
seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for the offset 1, but return 0 safely */
        return (seq_t)0;

    idx_t count = 0;           /* how many qualifying numbers have been seen */
    uint64_t k = 1;            /* candidate integer k (fits in 64‑bit for n ≤ 10000) */

    while (count < n) {
        /* Find the odd part of k: strip all trailing zeros. */
        uint64_t odd = k;
        while ((odd & 1ULL) == 0ULL)
            odd >>= 1;

        /* Test  odd < sqrt(2 k).  Equivalent to odd*odd < 2*k. */
        /* Use 128‑bit arithmetic to avoid overflow on large intermediates. */
        __uint128_t lhs = (__uint128_t)odd * odd;
        __uint128_t rhs = (__uint128_t)2 * k;
        if (lhs < rhs) {          /* condition satisfied */
            ++count;
            if (count == n)       /* reached the desired index */
                return (seq_t)k;
        }
        ++k;                      /* next candidate */
    }

    /* Unreachable for valid inputs.  */
    return (seq_t)0;
}
