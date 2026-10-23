/*
 * A032532  Integer part of decimal 'base-2 looking' numbers divided by their
 *          actual base-2 values (denominator of a(n) is n, numerator is n
 *          written in binary but read in decimal).
 * https://oeis.org/A032532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(A007088(n)/n) where A007088(n) is the * decimal
 *            number obtained by writing the binary representation of n * in
 *            base‑10
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The function is defined with external linkage as requested. */
seq_t a(idx_t n)
{
    /* Undefined for n == 0; the OEIS offset starts at 1. */
    if (n == 0) return 0;

    /* Find the most significant set bit: msb = floor(log2(n)). */
    /* __builtin_clzll counts leading zeros in an unsigned long long. */
    const idx_t msb = 63 - __builtin_clzll((unsigned long long)n);

    /* Remainder during the division process – big enough to hold
     * r * 10 + bit (r < n <= 2^64-1). */
    unsigned __int128 rem = 0;

    /* Quotient built incrementally, using the base‑10 division algorithm. */
    seq_t q = 0;

    /* Flag indicating whether a non‑zero quotient digit has been appended
     * so that we can skip leading zeros. */
    int started = 0;

    for (idx_t pos = msb; pos <= msb; --pos) {  /* loop until pos underflows */
        /* Extract the current bit (MSB first). */
        const unsigned int bit = (unsigned int)((n >> pos) & 1ULL);
        /* temp = rem * 10 + bit; temp can exceed 64 bits, so use 128 bits. */
        unsigned __int128 temp = rem * 10 + bit;
        /* q_digit is the next decimal digit of the quotient. */
        const unsigned int q_digit = (unsigned int)(temp / n);
        rem = (unsigned long long)(temp % n);

        if (!started) {
            /* Skip leading zeros. */
            if (q_digit != 0) {
                started = 1;
                q = q * 10 + q_digit;
            }
        } else {
            q = q * 10 + q_digit;
        }

        /* Stop looping when pos is zero; the for loop condition ensures that. */
        if (pos == 0) break;
    }

    /* For n = 1 the loop may skip appending any digit because q_digit would
     * be 1 and we appended it, so the result is correct.  For n > 1 the
     * quotient cannot be zero (except for n = 1).  Returning q as seq_t
     * gives the floor division result. */
    return q;
}
