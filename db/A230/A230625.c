/*
 * A230625  Concatenate prime factorization written in binary, convert back to
 *          decimal.
 * https://oeis.org/A230625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 6459, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A230625: a(n) = Concatenate prime factorization written in binary, then convert back to decimal.
   For n = 1 output 1. For n > 1 factorize n, write each prime (and exponent if >1) in binary
   without separators, concatenate the bit sequences, then interpret the resulting binary
   string as an integer in base 10.  */
#include <stdint.h>

/* Compute a(n) for this OEIS sequence. */
seq_t a(idx_t n)
{
    if (n == 1)                 /* defined specially in the OEIS sequence */
        return 1;

    seq_t  result = 0;          /* accumulated binary number as a seq_t */
    uint64_t temp = (uint64_t)n;/* working copy for factorization  */

    /* trial‑division factorization */
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            /* count exponent of this prime */
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }

            /* prime in binary: p, length = bitlen(p) */
            uint64_t primebits = p;
            int primelen = 0;
            uint64_t q = p;
            while (q) { primelen++; q >>= 1; }
            if (primelen == 0) primelen = 1;            /* just in case */

            result = (result << primelen) | (seq_t)primebits;

            /* exponent in binary if > 1 */
            if (exp > 1) {
                uint64_t expbits = exp;
                int explen = 0;
                uint64_t r = exp;
                while (r) { explen++; r >>= 1; }
                if (explen == 0) explen = 1;

                result = (result << explen) | (seq_t)expbits;
            }
        }
    }

    /* remaining prime factor (if any) */
    if (temp > 1) {
        /* exponent is 1, so only prime bits are added */
        uint64_t primebits = temp;
        int primelen = 0;
        uint64_t r = temp;
        while (r) { primelen++; r >>= 1; }
        if (primelen == 0) primelen = 1;

        result = (result << primelen) | (seq_t)primebits;
    }

    return result;
}
