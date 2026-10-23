/*
 * A173556  Triangle T(n,m) = (prime(n)-1)*(prime(m)- 1) mod 8 read by rows.
 * https://oeis.org/A173556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (prime(i) - 1)*(prime(j) - 1) mod 8, where the pair (i,j)
 *            is determined by the position of n in the row‑wise traversal of
 *            the triangle T(r,c) = (prime(r) - 1)*(prime(c) - 1) mod 8
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Flags and helpers ---------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* Limited integer arithmetic with 128‑bit for safety ----------------- */
/* Return the r‑th triangular number: r*(r+1)/2, safe up to 64‑bit.     */
static inline idx_t triangular(idx_t r)
{
    __uint128_t t = (__uint128_t)r * (r + 1) / 2;
    return (idx_t)t;
}

/* Return the smallest r such that triangular(r) >= n.  Binary search. */
static inline idx_t row_for_index(idx_t n)
{
    idx_t lo = 1, hi = n * 2;          /* hi is a safe upper bound */
    while (lo < hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (triangular(mid) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Main sequence generator ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Treat bogus input never encountered in the problem domain */
    if (n < 1) return (seq_t)0;

    /* Find the row r and column c in the triangle */
    idx_t r = row_for_index(n);
    idx_t c = n - triangular(r - 1);   /* 1‑based column */

    /* Retrieve the relevant primes */
    uint32_t pr = get_prime(r);
    uint32_t pc = get_prime(c);

    /* Compute (prime-1) modulo 8 for each factor */
    uint32_t a_r = (pr - 1) & 7u;
    uint32_t a_c = (pc - 1) & 7u;

    /* Product modulo 8 */
    uint32_t prod = (a_r * a_c) & 7u;

    return (seq_t)prod;
}
