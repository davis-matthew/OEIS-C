/*
 * A173292  a(n)! has exactly 10^n trailing zeros (a(n) = -1 if no such number
 *          exists).
 * https://oeis.org/A173292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 8, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A173292: find the smallest integer N such that N! has exactly 10^n trailing zeros.
    The routine uses a binary search on N.  It returns -1 if no such N exists
    (i.e. if the desired number of zeros is skipped by the trailing‑zero function). */
#include "../utils/pow10.h"

static seq_t trailing_zeros(seq_t N)
{
    seq_t sum = 0;
    for (seq_t d = N / 5; d > 0; d /= 5)
        sum += d;
    return sum;
}

seq_t a(idx_t n)
{
    /* Target number of trailing zeros */
    seq_t T = pow10(n);

    /* Upper bound for the search: 5*T guarantees t(5T) >= T */
    seq_t low  = 0;
    seq_t high = T * 5;          /* multiplication safe for seq_t (big‑int or 128‑bit) */

    /* Binary search for minimal N with trailing_zeros(N) >= T */
    while (low < high) {
        seq_t mid = (low + high) / 2;
        seq_t tz  = trailing_zeros(mid);
        if (tz >= T)
            high = mid;
        else
            low  = mid + 1;
    }

    /* Verify that the found N actually attains the target */
    if (trailing_zeros(low) == T)
        return low;
    else
        return (seq_t)-1;   /* no integer with exactly T trailing zeros */
}
