/*
 * A062471  Integer part of log(n^n)^(1 + log(n)/n).
 * https://oeis.org/A062471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (n*ln(n))^(1 + ln(n)/n) )
 * Range:     u8 n <= 43, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>  /* for log() and powl() */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n==0 return 0. */
    if (n == 0)
        return 0;

    /* Natural logarithm of n as a long double */
    long double ln_n = log((long double)n);

    /* n = 1 gives ln(n) = 0, hence the expression is 0 */
    if (ln_n == 0.0L)
        return 0;

    /* L = n·ln(n)  and  exp = 1 + ln(n)/n  */
    long double L     = (long double)n * ln_n;
    long double exp   = 1.0L + ln_n / (long double)n;

    /* Evaluate the real value using high‑precision (long double) */
    long double val = powl(L, exp);

    /* Convert the approximate value to an unsigned 64‑bit integer.
       The result for n ≤ 10 000 is far below 2^64, so the cast is
       safe for all seq_t implementations (uint8_t … unsigned __int128
       and arbitrary‑precision). */
    uint64_t candidate = (uint64_t)val;

    /* Correct possible rounding errors: adjust up or down until
       candidate < val < candidate+1 holds.  The loop is guaranteed
       to terminate after at most a few iterations because the error
       of a long double is far less than 1. */
    while ((long double)candidate > val)
        candidate--;
    while (((long double)(candidate + 1)) <= val)
        candidate++;

    /* Return the result as the build‑specific seq_t type. */
    return (seq_t)candidate;
}
