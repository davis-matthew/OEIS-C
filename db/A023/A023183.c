/*
 * A023183  a(n) = least k such that Fibonacci(k) ends with n, or -1 if there
 *          are none.
 * https://oeis.org/A023183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) = least k such that Fibonacci(k) ends with n, or -1 if none
 * Range:     i8 n <= 11, i16 n <= 9999, i32 n <= 9999, i64 n <= 9999,
 *            i128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches the Fibonacci sequence modulo 10^d,
   where d is the number of decimal digits of n.  The Pisano period
   for 10^d is 60 for d=1, 300 for d=2, and 15·10^(d-1) for d≥3.
   The search stops when the period is reached, guaranteeing that
   -1 is returned if no such k exists. */

#include <stdint.h>

/* Compute 10^exp as seq_t. */
static seq_t pow10_seq(int exp)
{
    seq_t r = 1;
    for (int i = 0; i < exp; ++i)
        r *= 10;
    return r;
}

/* Count decimal digits of an idx_t value; treat 0 as one digit. */
static int digit_count_idx(idx_t x)
{
    int d = 1;
    while (x >= 10) {
        x /= 10;
        ++d;
    }
    return d;
}

seq_t a(idx_t n)
{
    /* Special case for zero. */
    if (n == 0)
        return 0;

    /* Number of decimal digits of n. */
    int d = digit_count_idx(n);

    /* Modulus 10^d. */
    seq_t mod = pow10_seq(d);
    seq_t target = (seq_t)n;

    /* Determine Pisano period for modulus 10^d. */
    seq_t period;
    if (d == 1)
        period = 60;
    else if (d == 2)
        period = 300;
    else
        period = (seq_t)15 * pow10_seq(d - 1);

    /* Convert period to idx_t for the loop bound. */
    idx_t periodIdx = (idx_t)period;

    /* Iterate Fibonacci modulo mod. */
    seq_t f0 = 0;
    seq_t f1 = 1;

    if (target == f0)
        return 0;
    if (target == f1)
        return 1;

    for (idx_t k = 2; k <= periodIdx; ++k) {
        seq_t f2 = (f0 + f1) % mod;
        if (f2 == target)
            return k;
        f0 = f1;
        f1 = f2;
    }

    /* Not found within one Pisano period → none exists. */
    return -1;
}
