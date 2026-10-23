/*
 * A117484  Number of triangular numbers mod n.
 * https://oeis.org/A117484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 2802, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A117484 – Number of triangular numbers mod n
   The sequence is multiplicative. For n = 2^e we have a(n) = 2^e.
   For an odd prime power p^e, a(n) = floor(p^(e+1)/(2p+2)) + 1.
   The implementation factors n by trial division and evaluates the
   formula directly.  All arithmetic is performed on the seq_t type. */

#include <stdint.h>   /* for uint64_t, if needed, but seq_t and idx_t are defined elsewhere */

seq_t a(idx_t n)
{
    if (n == 0)           /* the sequence is defined for n ≥ 1; guard against accidental 0 */
        return 1;

    seq_t result = 1;
    idx_t temp = n;
    idx_t exp;

    /* Handle factor 2 separately */
    exp = 0;
    while ((temp & 1) == 0) {
        ++exp;
        temp >>= 1;
    }
    if (exp > 0) {
        seq_t two_pow = 1;
        for (idx_t i = 0; i < exp; ++i)
            two_pow = two_pow * 2;
        result = result * two_pow;
    }

    /* Trial division for odd primes */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p != 0)
            continue;

        exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        /* Compute contribution for p^exp */
        seq_t num = 1;
        for (idx_t i = 0; i < exp + 1; ++i)
            num = num * (seq_t)p;          /* p^(exp+1) */

        seq_t denom = (seq_t)(2 * (seq_t)p + 2);  /* 2p + 2 */
        seq_t contrib = num / denom + 1;
        result = result * contrib;
    }

    /* Remaining prime factor (if any) */
    if (temp > 1) {
        exp = 1;   /* its exponent is 1 */

        seq_t num = (seq_t)temp * (seq_t)temp;     /* p^2 */
        seq_t denom = (seq_t)(2 * (seq_t)temp + 2);
        seq_t contrib = num / denom + 1;
        result = result * contrib;
    }

    return result;
}
