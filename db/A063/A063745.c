/*
 * A063745  Even numbers with an even number of prime factors (counted with
 *          multiplicity).
 * https://oeis.org/A063745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A063745 : even numbers with an even number of prime factors
   (counted with multiplicity). */

#include <stdint.h>

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    unsigned long long value = 4;   /* first even number with even parity */
    unsigned long long found  = 0;

    while (1) {
        /* Count prime factors of 'value' with multiplicity. */
        unsigned long long x = value;
        unsigned long long cnt = 0;

        /* Even number: handle factor 2 separately. */
        while ((x & 1ULL) == 0) {
            ++cnt;
            x >>= 1;
        }

        /* Trial division for odd primes. */
        for (unsigned long long p = 3; p * p <= x; p += 2) {
            while (x % p == 0) {
                ++cnt;
                x /= p;
            }
        }
        if (x > 1) ++cnt;  /* remaining prime factor */

        if ((cnt & 1ULL) == 0) {   /* even number of prime factors */
            ++found;
            if (found == n)
                return (seq_t) value;
        }
        value += 2;   /* next even number */
    }
}
