/*
 * A235944  Digital roots of squares of Lucas numbers.
 * https://oeis.org/A235944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = digital_root( Lucas(n)^2 ) Lucas(n) is the Lucas
 *            sequence: L0 = 2, L1 = 1, L_{k+1} = L_k + L_{k-1}. Digital root
 *            of a positive integer x is 1 + ((x-1) mod 9); equivalently, it
 *            is x mod 9 unless that residue is 0, in which case the result is
 *            9. Since only residues modulo 9 are needed, all a...
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   fib_mod9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fib_mod9.h"

seq_t a(idx_t n)
{
    /* Lucas(n) mod 9 = 2*F_{n+1} - F_n (in modulo 9)           */
    seq_t f, f1;
    fib_mod9(n, &f, &f1);                  /* f  = F_n   mod 9
                                                f1 = F_{n+1} mod 9 */

    seq_t luca = (f1 * 2) % 9;              /* 2*F_{n+1}   mod 9 */
    luca = (luca + 9 - f) % 9;              /* 2*F_{n+1} - F_n mod 9 */

    /* Square modulo 9 */
    seq_t sq = (luca * luca) % 9;           /* (L_n)^2 mod 9   */

    /* Digital root of the square: 0 residue corresponds to 9 */
    if (sq == 0) {
        return (seq_t)9;
    }
    return sq;
}
