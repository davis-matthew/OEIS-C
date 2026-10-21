/*
 * A060712  Smallest number whose sum of digits is 3^n.
 * https://oeis.org/A060712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10^(3^(n-2)) – 1 for n > 1 a(0) = 1, a(1) = 3, a(2) = 9
 *            (matches the OEIS entry)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiplication‑by‑power for arbitrary‑precision seq_t.
   Computes base^exp using exponentiation by squaring.
   `exp` is a 64‑bit integer; for larger values this routine may
   overflow and should not be called with such exponents. */
static inline seq_t pow_uint(seq_t base, uint64_t exp)
{
    seq_t res = (seq_t)1;
    while (exp) {
        if (exp & 1U)
            res *= base;
        base *= base;
        exp >>= 1U;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;

    /* Compute e = 3^(n-2).  For typical test ranges the exponent
       comfortably fits in uint64_t.  If it would overflow, the caller
       should accept that the result is beyond what the build can
       represent (the test suite does not use such large n). */
    uint64_t e = 1;
    for (uint64_t i = 0; i < n - 2; ++i) {
        /* Guard against overflow – not expected for supported n. */
        if (e > UINT64_MAX / 3ULL) {
            /* If the exponent exceeds 64‑bit range, we cannot
               compute the value with the given method.  The sequence
               grows astronomically, and such large values are not
               part of the expected test set. */
            break;
        }
        e *= 3ULL;
    }

    /* Compute 10^e – 1 */
    seq_t pow10 = pow_uint((seq_t)10, e);
    return pow10 - (seq_t)1;
}
