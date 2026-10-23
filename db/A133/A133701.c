/*
 * A133701  Triangle read by rows: T(n, k) = A001227(n) if k divides n, T(n,
 *          k) = 0 otherwise.
 * https://oeis.org/A133701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the nth entry of the triangle T(r,c) defined by * T(r,c)
 *            = odd_divisor_count(r) if c divides r, otherwise 0. * The
 *            triangle is read by rows, with offset 1. * odd_divisor_count(r)
 *            = number of odd divisors of r. * * The index n (starting from 1)
 *            is first converted to its * (row r, position c...
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdio.h>

/* Assume seq_t and idx_t are defined in the build environment. */

static seq_t odd_divisor_count(idx_t n) {
    /* Count divisors of n that are odd.  Extract the odd part first. */
    idx_t m = n;
    while ((m & 1ULL) == 0) {
        m >>= 1;
    }
    seq_t res = 1;
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            seq_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            res *= (exp + 1);
        }
    }
    if (m > 1) {
        res *= 2;
    }
    return res;
}

static idx_t find_row(idx_t n) {
    /* Binary search for the smallest r such that r(r+1)/2 >= n. */
    idx_t lo = 1, hi = 1;
    /* Grow hi until it is large enough. */
    while ((__int128)hi * (hi + 1) / 2 < n) {
        hi <<= 1;
        if (hi == 0) {   /* overflow protection */
            hi = (idx_t)1 << 63;
            break;
        }
    }
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* safety for n < 1 */
    idx_t r = find_row(n);
    idx_t prev_sum = (idx_t)((__int128)r * (r - 1) / 2);
    idx_t pos = n - prev_sum;    /* 1‑based position in row r */

    if (r % pos == 0)
        return odd_divisor_count(r);
    else
        return (seq_t)0;
}
