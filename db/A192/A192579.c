/*
 * A192579  Primes p for which there is no prime q == 3 (mod 4) that is
 *          smaller than p and is a quadratic residue modulo p.
 * https://oeis.org/A192579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A192579 – primes p for which no smaller prime q ≡ 3 (mod 4) is a quadratic
    residue modulo p.  By Gica’s result the sequence is finite and consists of
    exactly the primes 2, 3, 5, 7, 17.  Therefore a(n) is 0 for n > 5. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    /*  Offset 1.  a(1) = 2, a(2) = 3, a(3) = 5, a(4) = 7, a(5) = 17. */
    static const seq_t values[5] = {
        (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)7, (seq_t)17
    };

    if (n == 0) return (seq_t)0;        /* never used – offset is 1 */
    if (n <= 5) return values[n-1];
    return (seq_t)0;                    /* no further terms */
}
