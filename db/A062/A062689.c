/*
 * A062689  Smallest square that contains the digits of n in its exact middle.
 * https://oeis.org/A062689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 1056, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 57 terms through n = 9863 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) = smallest square whose decimal representation contains the decimal
 * digits of n in its exact centre.
 *
 * This function searches for the desired square by examining all possible
 * lengths L = d + 2*t of the square (d = number of digits of n) and, for
 * each length, iterating over all k whose square has exactly that many
 * digits.  For each k the middle d digits are compared with n.
 *
 * The algorithm stops as soon as a matching square is found.  It also
 * protects against overflow when seq_t is an unsigned 64‑bit integer:
 * the search stops once the lower bound of the square would exceed the
 * maximum representable value, which happens before the search would
 * otherwise wrap around.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Integer square root (floor) of a non‑negative seq_t value. */
static seq_t isqrt_seq(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

static bool pow10_overflow(idx_t exp, seq_t *res)
{
    /* Computes 10^exp into *res.  Returns true if overflow would occur
     * when seq_t is a bounded unsigned integer type. */
    static const seq_t MAX_VAL =
#if defined(UINT64_MAX)
        (seq_t)UINT64_MAX;
#else
        0;  /* 0 indicates unbounded seq_t */
#endif

    seq_t val = 1;
    for (idx_t i = 0; i < exp; ++i) {
        if (sizeof(seq_t) <= sizeof(UINT64_MAX) && MAX_VAL > 0 && val > MAX_VAL / 10)
            return true;      /* overflow would happen */
        val *= 10;
    }
    *res = val;
    return false;
}

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0) return (seq_t)0;

    /* Convert n to seq_t for digit operations */
    seq_t target = (seq_t)n;

    /* Number of decimal digits of n */
    int d = num_digits(target);

    /* Determine whether seq_t is a bounded unsigned integer (<= 8 bytes) */
    static const bool BOUNDED = (sizeof(seq_t) == sizeof(uint64_t));

    /* Maximum representable value of seq_t (defined only when BOUNDED) */
    const seq_t MAX_VAL = BOUNDED ? (seq_t)UINT64_MAX : 0;

    seq_t pow10_d;
    if (pow10_overflow(d, &pow10_d))
        return (seq_t)0;                 /* cannot represent required power */

    /* Search over possible t (number of digits on each side of n) */
    for (int t = 0;; ++t) {
        int L = d + 2 * t;               /* total number of digits of the square */

        /* Compute lower = 10^(L-1) and upper = 10^L - 1 */
        seq_t low, high;
        if (pow10_overflow(L - 1, &low))
            break;                      /* lower bound already exceeds capacity */
        if (pow10_overflow(L, &high)) {
            /* high would overflow, so set high to MAX_VAL */
            high = MAX_VAL;
        } else {
            high -= 1;                  /* subtract 1 safely */
        }

        /* If low > high, no squares of this length exist */
        if (low > high)
            continue;

        /* Determine k range: sqrt(low) <= k <= sqrt(high) */
        seq_t kmin = isqrt_seq(low - 1) + 1;
        seq_t kmax = isqrt_seq(high);

        /* Precompute shift and 10^shift for this t */
        int shift = t;
        seq_t pow10_shift;
        if (pow10_overflow(shift, &pow10_shift))
            continue;                    /* t too large for this seq_t */
        seq_t pow10_second = pow10_shift * pow10_d; /* 10^(t+d) */

        /* Iterate over all k in the range */
        for (seq_t k = kmin; k <= kmax; ++k) {
            seq_t sq = k * k;

            /* Check for overflow: if sq < k, multiplication wrapped */
            if (BOUNDED && sq < k)
                break;                    /* further k would produce larger squares */

            /* Extract middle digits */
            seq_t mid = sq / pow10_shift;
            if ((mid % pow10_d) == target)
                return sq;
        }
    }

    /* If the function reaches here, no square fits within the representable
       range of seq_t for this n (which can happen when seq_t is bounded and
       the answer requires a larger integer).  The caller should only rely on
       this function for n whose answer fits in seq_t. */
    return (seq_t)0;
}
