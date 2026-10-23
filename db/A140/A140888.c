/*
 * A140888  Number of nonprimes in [210n - 210, 210n] coprime to 210.
 * https://oeis.org/A140888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = # of integers x in [210n‑210, 210n] such that
 *            gcd(x,210)=1 and x is not prime (1 counted as non‑prime)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* residues modulo 210 that are coprime to 210, 48 values total */
static const idx_t coprime210[48] = {
    1, 11, 13, 17, 19, 23, 29, 31, 37, 41,
    43, 47, 53, 59, 61, 67, 71, 73, 79, 83,
    89, 97,101,103,107,109,113,121,127,131,
   137,139,143,149,151,157,163,167,169,173,
   179,181,187,191,193,197,199,209
};

seq_t a(idx_t n)
{
    idx_t base = 210 * (n - 1);       /* starting point of the block */
    seq_t cnt = 0;

    for (int i = 0; i < 48; ++i) {
        idx_t val = base + coprime210[i];
        if (!is_prime(val))          /* counts 1 as non‑prime, composites as well */
            ++cnt;
    }
    return (seq_t)cnt;
}
