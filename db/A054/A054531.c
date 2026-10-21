/*
 * A054531  Triangular array T read by rows: T(n,k) = n/gcd(n,k) (n >= 1, 1 <=
 *          k <= n).
 * https://oeis.org/A054531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, frac, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 7260, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A054531:  T(n,k) = n / gcd(n,k) read by rows
 *  a(n) is the element in the triangular array returned in row-major
 *  order.  For a given index n (1‑based) find the row r and column k
 *  with 1 ≤ k ≤ r such that  
 *       (r-1)r/2 < n ≤ r(r+1)/2 ,
 *  then a(n) = r / gcd(r,k), where k = n - (r-1)r/2.
 *  The implementation uses only integer arithmetic and a simple
 *  Euclidean gcd.  No external headers that are not guaranteed to be
 *  present are used.
 */

#include <stdint.h>

/* Euclidean algorithm for unsigned 64‑bit integers (does not use
 * any of the pre‑supplied helpers, and therefore does not conflict
 * with the build's seq_t definitions).  The arguments are
 * guaranteed to be non‑zero because k ∈ [1,r]. */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;   /* remainder */
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; the offset of the sequence is 1. */
    if (n == 0) {
        /* Undefined by OEIS; return 0 for safety. */
        return (seq_t)0;
    }

    /* --- find the row r such that (r-1)r/2 < n ≤ r(r+1)/2 --- */
    idx_t low  = 1;
    idx_t high = n;          /* high is always an upper bound */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        /* Use 128‑bit intermediate to avoid overflow of mid*(mid+1)/2 */
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t r = low;

    /* --- compute the column k --- */
    /* prev = (r-1)r/2; use 128‑bit to avoid overflow. */
    __uint128_t prev = (__uint128_t)(r - 1) * r / 2;
    idx_t   k = (idx_t)(n - prev);

    /* --- result:  r / gcd(r,k) --- */
    uint64_t g = gcd_uint64(r, k);
    return (seq_t)(r / g);
}
