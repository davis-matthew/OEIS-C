/*
 * A033778  Product t2(q^d); d | 19, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = #{(x,y) ∈ ℕ^2 : x^2 + 19*y^2 = 8*n + 20,  x,y odd}
 * Equivalent: count non‑negative integer pairs (a,b) with a(a+1)+19*b(b+1)=2*n
 * (x=2a+1, y=2b+1).
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t target = 2ULL * (uint64_t)n;          /* 2*n  */
    uint64_t count  = 0;                          /* number of solutions */

    for (uint64_t b = 0; ; ++b) {
        uint64_t t1 = 19ULL * b * (b + 1);        /* 19*b(b+1)  */
        if (t1 > target) break;                  /* no further solutions */

        uint64_t rem = target - t1;              /* remaining for a(a+1) */
        uint64_t d   = 1 + 4ULL * rem;           /* discriminant 1 + 4*rem */

        /* integer square root of d, small enough to brute‑force */
        uint64_t s = 0;
        while ((s + 1) * (s + 1) <= d) ++s;

        if (s * s == d) {
            /* a = (s-1)/2 is a non‑negative integer   */
            ++count;
        }
    }

    return (seq_t)count;
}
