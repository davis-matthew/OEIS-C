/*
 * A070620  a(n) = n^5 mod 37.
 * https://oeis.org/A070620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 37
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* a(n) returns n^5 modulo 37.  The function is defined with external
   linkage and uses only arithmetic on small integer types to avoid
   overflow before the final cast to seq_t. */
seq_t a(idx_t n)
{
    /* Reduce the index modulo 37 – this is the only information that
       influences the result. */
    unsigned long long m = n % 37ULL;   /* m ∈ [0,36] */

    /* Compute m^5 mod 37 using repeated multiplication.  The product
       of two numbers < 37 fits easily into unsigned long long, so we
       can safely perform the multiplication before taking the modulus. */
    unsigned long long r = 1ULL;          /* r will always be < 37 */
    for (int i = 0; i < 5; ++i)
        r = (r * m) % 37ULL;

    return (seq_t)r;
}
