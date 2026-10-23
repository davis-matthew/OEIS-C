/*
 * A138825  Divisors of 16775168 (the 5th perfect number divided by 2),
 *          written in base 2.
 * https://oeis.org/A138825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, nonn, less
 * Formula:   a(n) = 10^(n-1) for 1 ≤ n ≤ 12 a(n) = ((10^13 – 1)/9) *
 *            10^(n-13) for 13 ≤ n ≤ 24 a(n) = 0 for n > 24 (no further
 *            divisors) (Sequence of divisors of 16775168 (2^11·8191) written
 *            in base 2.)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, if needed by users */

/* Compute 10^e as a seq_t value.  e is small (≤ 24). */
static seq_t pow10_local(idx_t e)
{
    seq_t val = (seq_t)1;
    for (idx_t i = 0; i < e; ++i) {
        val *= (seq_t)10;
    }
    return val;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;  /* below the offset – value irrelevant */
    }

    if (n <= 12) {
        /* 10^(n-1) gives the binary string 1 followed by n-1 zeros as a decimal number. */
        return pow10_local(n - 1);
    }

    if (n <= 24) {
        /* Number consisting of 13 consecutive binary ones:
           (10^13 – 1)/9 = 1111111111111 (decimal). */
        seq_t ones13 = (pow10_local(13) - (seq_t)1) / (seq_t)9;
        /* Append (n-13) zeros: multiply by 10^(n-13). */
        return ones13 * pow10_local(n - 13);
    }

    /* For n > 24, the divisor list has ended. */
    return (seq_t)0;
}
