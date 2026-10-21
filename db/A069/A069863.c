/*
 * A069863  a(n) is the shortest concatenation of two or more decreasing
 *          consecutive numbers (n)(n-1)...(0) such that n divides a(n), or n
 *          if there is no such concatenation.
 * https://oeis.org/A069863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) = the shortest concatenation of two or more decreasing
 *            consecutive numbers (n)(n-1)...(k) such that n divides the
 *            concatenated integer; if no such concatenation exists, a(n) = n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the number of decimal digits of an idx_t value.  */
static idx_t digits_idx(idx_t v)
{
    idx_t d = 0;
    if (v == 0) return 1;
    while (v) {
        ++d;
        v /= 10;
    }
    return d;
}

/* Return 10^e as a seq_t value (used for building the full integer).  */
static seq_t pow10_seq(idx_t e)
{
    seq_t r = 1;
    while (e-- > 0) r *= (seq_t)10;
    return r;
}

/* Return 10^e mod m (m fits in idx_t).  */
static idx_t pow10_mod(idx_t e, idx_t m)
{
    idx_t r = 1;
    while (e-- > 0) r = (r * 10) % m;
    return r;
}

/* Main OEIS sequence function.  */
seq_t a(idx_t n)
{
    /* Start with the first number alone.  We only return it if no
       longer prefix satisfies the divisibility condition.  */
    seq_t result = (seq_t)n;
    /* current remainder of the number we are building modulo n
       (n % n == 0).  */
    idx_t remainder = 0;

    /* Build successive prefixes n, n(n-1), n(n-1)(n-2), ... ; stop at 0.  */
    for (idx_t m = n - 1; ; --m) {
        /* Number of digits in the next integer m.  */
        idx_t digits = digits_idx(m);

        /* Update the remainder: (previous_remainder * 10^digits + m) mod n. */
        idx_t pow10_d_mod = pow10_mod(digits, n);
        remainder = ( (remainder * pow10_d_mod) + (m % n) ) % n;

        /* Update the full concatenated number: previous * 10^digits + m. */
        seq_t pow10_d = pow10_seq(digits);
        result = result * pow10_d + (seq_t)m;

        /* If at least two numbers have been concatenated and n divides the
           prefix, we have found the shortest suitable concatenation. */
        if (remainder == 0) return result;

        /* Stop after reaching 0.  */
        if (m == 0) break;
    }

    /* No concatenation with at least two numbers is divisible by n.  */
    return (seq_t)n;
}
