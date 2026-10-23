/*
 * A191780  Triangle G(n,k): the value of n written in base k with digits
 *          reversed (but written here in base 10) for 2 <= k <= n.
 * https://oeis.org/A191780
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th term of the flattened triangle G(i,k),
 * where G(i,k) is i written in base k with its digits reversed
 * (the reversed digits are interpreted as a base‑k number).  The
 * sequence starts at n = 2 with G(2,2) = 1. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2)          /* values below the offset are irrelevant */
        return 0;

    /* Find the row i (≥2) such that the cumulative number of entries
       up to row i-1 is less than n‑1 and up to row i is ≥ n‑1.
       The cumulative count up to row r equals r*(r-1)/2. */
    idx_t target = n - 1;          /* 1‑based target within cumulative counts */
    idx_t lo = 2, hi = 2;
    while (((__int128)hi * (hi - 1)) / 2 < target)
        hi <<= 1;                  /* ensure hi is large enough */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (((__int128)mid * (mid - 1)) / 2 >= target)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t row = lo;                /* row number i */

    /* Position within the row (0‑based). */
    idx_t prev = ((__int128)(row - 1) * (row - 2)) / 2; /* cumulative up to row-1 */
    idx_t pos   = (n - 2) - prev;                       /* n-2 is 0‑based global index */
    idx_t k    = pos + 2;                                /* base ranges from 2 to row */

    /* Reverse the representation of 'row' in base k, interpreting
       the reversed digits as a base‑k number. */
    seq_t rev  = 0;
    seq_t base = (seq_t)k;
    idx_t temp = row;
    while (temp > 0) {
        idx_t digit = temp % k;
        rev = rev * base + (seq_t)digit;
        temp /= k;
    }
    return rev;
}
