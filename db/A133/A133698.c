/*
 * A133698  Triangle, diagonal = A001227 with the rest zeros.
 * https://oeis.org/A133698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9335, u16 n <= 9335, u32 n <= 9335, u64 n <= 9335,
 *            u128 n <= 9335, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of odd divisors of r
 * if n is the r-th triangular number T_r = r*(r+1)/2,
 * otherwise a(n) = 0.  The sequence is obtained by
 * flattening the lower triangular matrix with
 * diagonal A001227 (odd divisor count) and
 * zeros elsewhere.
 */

#include <stdbool.h>
#include <stdint.h>

/* ---------- local helpers --------------------------------------------- */

/* Integer square root of a 128‑bit unsigned integer.
 * Returns the floor of sqrt(x).  Uses binary search. */
static idx_t isqrt_u128(__int128 x)
{
    idx_t low = 0, high = 0x100000000ULL; /* 2^32 is enough for sqrt(8*2^64) */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((__int128)mid * mid <= x)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* Count the number of odd divisors of n.
 * Returns the count as a seq_t value. */
static seq_t odd_div_count(idx_t n)
{
    /* Remove all factors of 2. */
    while ((n & 1ULL) == 0)
        n >>= 1;

    seq_t cnt = 1;
    idx_t remaining = n;

    /* Trial division by odd primes. */
    for (idx_t p = 3; p * p <= remaining; p += 2) {
        if (remaining % p == 0) {
            seq_t exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                exp++;
            }
            cnt *= (exp + 1); /* multiply by (exponent + 1) */
        }
    }

    /* If a prime factor > sqrt remains. */
    if (remaining > 1)
        cnt *= 2;            /* prime factor contributes exponent 1 */

    return cnt;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1; safe guard */
        return 0;

    /* Find r such that T_r = r*(r+1)/2 >= n. */
    __int128 x = (__int128)8 * n + 1;
    idx_t s = isqrt_u128(x);
    idx_t r = (s - 1) / 2;

    /* Correct r if triangular number is too small. */
    seq_t tri = ((seq_t)r * (r + 1)) / 2;
    if (tri < (seq_t)n) {
        /* r too small, increase until adequate */
        do {
            r++;
            tri = ((seq_t)r * (r + 1)) / 2;
        } while (tri < (seq_t)n);
    } else if (r > 0) {
        /* r may be too large; decrease if necessary */
        seq_t prevTri = ((seq_t)(r - 1) * r) / 2;
        while (prevTri >= (seq_t)n) {
            r--;
            prevTri = ((seq_t)(r - 1) * r) / 2;
        }
    }

    /* If n is exactly a triangular number, return odd divisor count of r. */
    if (tri == (seq_t)n)
        return odd_div_count(r);
    else
        return 0;
}
