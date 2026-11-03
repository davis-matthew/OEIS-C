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
 * Validated: partial, pass, 108 terms through n = 107 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute F_n and F_{n+1} modulo 9 using fast doubling.
   All intermediate results are kept modulo 9. */
static void fib_mod9(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t f, g;
    fib_mod9(n >> 1, &f, &g);

    /* f = F_k mod 9, g = F_{k+1} mod 9   (k = n/2) */
    seq_t two_g   = (g * 2) % 9;           /* 2*F_{k+1}      */
    seq_t temp    = (two_g + 9 - f) % 9;   /* (2*F_{k+1} - F_k) mod 9 */
    seq_t c       = (f * temp) % 9;        /* F_{2k}          */
    seq_t d       = (g * g + f * f) % 9;   /* F_{2k+1}        */

    if ((n & 1ULL) == 0ULL) {
        *fn  = c;           /* F_n      */
        *fn1 = d;           /* F_{n+1}  */
    } else {
        *fn  = d;           /* F_n      */
        *fn1 = (c + d) % 9; /* F_{n+1}  */
    }
}

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
