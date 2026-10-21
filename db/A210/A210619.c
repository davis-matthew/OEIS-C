/*
 * A210619  Triangle of numbers with n 1's and n 0's in their representation
 *          in base of Fibonacci numbers (A014417).
 * https://oeis.org/A210619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 66, u32 n <= 254, u64 n <= 1036,
 *            u128 n <= 1225, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Fibonacci(2*r+2) - Fibonacci(2*(r-k)) - 1
 * where r is the smallest integer with r*(r+1)/2 ≥ n
 *      and k = n - r*(r-1)/2 (the 1‑based position within that row).
 * This equals the nth integer having an equal number of 1’s and 0’s
 * in its Zeckendorf (Fibonacci base) representation.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Sequence is defined for n ≥ 1; return 0 for the unused offset. */
        return (seq_t)0;
    }

    /* Find the row r such that r(r+1)/2 ≥ n. */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* Position within the r‑th row, 1‑based. */
    idx_t prev = (r * (r - 1)) / 2;
    idx_t k = n - prev;

    /* Compute T(r,k) = Fib(2r+2) - Fib(2(r-k)) - 1.
     * For k == r, Fib(0) in the formula should be 0, not fib(0)
     * (the utilities may define fib(0) = 1).  Handle that explicitly. */
    seq_t f1 = fib(2 * r + 2);
    seq_t f2;
    if (k == r) {
        f2 = (seq_t)0;
    } else {
        f2 = fib(2 * (r - k));
    }

    return f1 - f2 - (seq_t)1;
}
