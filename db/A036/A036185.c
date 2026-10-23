/*
 * A036185  Log base 3 (n) mod 113.
 * https://oeis.org/A036185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete_log_3(n mod 113) (with a(n)=0 if n≡0 mod 113)
 * Range:     u8 n <= 112, u16 n <= 112, u32 n <= 112, u64 n <= 112,
 *            u128 n <= 112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the discrete logarithm of n to base 3 modulo the prime 113.
   The function precomputes a mapping from residues 0..112 to their
   logarithm exponents (0..111) the first time it is called, and then
   returns the appropriate exponent modulo 113 for any subsequent call. */
#include <stddef.h>
#include <stdbool.h>

/* Return a(n) for n >= 1. */
seq_t a(idx_t n)
{
    /* Mapping from residue to discrete log exponent. */
    static seq_t exp_of[113];
    static bool ready = false;

    if (!ready) {
        /* 3 is a primitive root modulo 113.  Populate the table. */
        seq_t value = 1;   /* 3^0 mod 113 = 1 */
        exp_of[1] = 0;
        for (idx_t k = 1; k < 112; ++k) {
            /* Multiply by 3 and reduce modulo 113. */
            value = (value * 3u) % 113u;
            exp_of[(size_t)value] = (seq_t)k;
        }
        exp_of[0] = 0;     /* 0 has no logarithm; return 0 by convention. */
        ready = true;
    }

    /* Reduce n modulo 113. */
    size_t r = (size_t)(n % 113u);

    if (r == 0)
        return 0;              /* convention for multiples of 113 */
    return exp_of[r];
}
