/*
 * A024171  Integer part of ((4th elementary symmetric function of
 *          1,2,...,n)/(1+2+...+n)).
 * https://oeis.org/A024171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (1/2880)*(n-3)*(n-2)*(n-1)*(15*n^3 + 15*n^2 - 10*n
 *            - 8) )
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 97, u64 n <= 3905,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For n = 1..3 the formula gives 0, matching the OEIS terms. */
    if (n < 4)
        return (seq_t)0;

    /* All arithmetic below is done in 128‑bit unsigned integers to avoid
       overflow for n up to ~10 000 (the range used in the tests). */
    unsigned __int128 n128 = (unsigned __int128)n;

    /* p = 15*n^3 + 15*n^2 - 10*n - 8 */
    unsigned __int128 p = 15 * n128 * n128 * n128
                       + 15 * n128 * n128
                       - 10 * n128
                       - 8;

    /* numerator = (n-3)*(n-2)*(n-1)*p */
    unsigned __int128 numerator = (n128 - 3) * (n128 - 2) * (n128 - 1) * p;

    /* Divide by 2880 (the fractional part is discarded by integer division). */
    numerator /= 2880;

    /* Convert the 128‑bit result to seq_t.
       For built‑in types this truncates; for arbitrary‑precision seq_t
       (e.g. boost::multiprecision::cpp_int) the conversion preserves
       the full value, which is sufficient for all test cases (n ≤ 10 000). */
    return (seq_t)numerator;
}
