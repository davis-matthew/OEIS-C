/*
 * A140891  Binary encoding of the prime-ness of the 6 integers r+14*n with
 *          remainder r=1, 3, 5, 9, 11 or 13.
 * https://oeis.org/A140891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Binary encoding of the prime-ness of the 6 integers  
   r+14*n with remainder r=1,3,5,9,11,13 (LSB r=1, MSB r=13).  
   a(n) = Σ_{k=0}^{5} [ 14*n + r_k is composite ] · 2^k.          */

/* The function relies on the generic primality test: */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Remainders in order of increasing bit position. */
    static const idx_t rs[6] = {1, 3, 5, 9, 11, 13};

    seq_t val = 0;                 /* result accumulator */
    for (int k = 0; k < 6; ++k) {
        idx_t num = 14 * n + rs[k];    /* the number to test */
        /* If not prime, set the corresponding bit. */
        if (!is_prime(num)) {
            val |= ((seq_t)1 << k);
        }
    }
    return val;
}
