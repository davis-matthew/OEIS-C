/*
 * A059293  a(n) = round(n*(5*n - 14)/12) + 1.
 * https://oeis.org/A059293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n*(5*n - 14)/12) + 1
 * Range:     u8 n <= 26, u16 n <= 397, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A059293. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert the index to a signed 64‑bit integer for safe intermediate
       arithmetic.  It is safe because the test data use n up to about 10^4. */
    int64_t N = (long long)n;
    int64_t num = N * (5 * N - 14);          /* n*(5*n-14) */
    int64_t q   = num / 12;                  /* truncating division */
    int64_t r   = num % 12;                  /* remainder keeps the sign of num */

    /* Rounding to the nearest integer:  carry out the rounding explicitly.
       When the remainder is  ±6 or larger, move to the next integer.
       For positive remainders we round up, for negative remainders we round
       down (i.e., toward more negative). */
    if (r >= 6) {
        q += 1;
    } else if (r <= -6) {
        q -= 1;
    }

    return (seq_t)(q + 1);   /* add the constant term of the formula */
}
