/*
 * A008832  Discrete logarithm of n to the base 2 modulo 19.
 * https://oeis.org/A008832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = discrete logarithm of n base 2 modulo 19, i.e. the
 *            smallest k such that 2^k ≡ n (mod 19). For n ≡ 0 (mod 19) no
 *            solution exists, the function returns 0 by convention
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* precomputed discrete logarithms for residues modulo 19 */
    static const seq_t log_table[19] = {
        0,  /* residue 0 (no solution) */
        0,  /* 2^0 mod 19 = 1  -> residue 1 */
        1,  /* 2^1 mod 19 = 2  -> residue 2 */
        13, /* 2^13 mod 19 = 3 -> residue 3 */
        2,  /* 2^2 mod 19 = 4  -> residue 4 */
        16, /* 2^16 mod 19 = 5 -> residue 5 */
        14, /* 2^14 mod 19 = 6 -> residue 6 */
        6,  /* 2^6 mod 19 = 7  -> residue 7 */
        3,  /* 2^3 mod 19 = 8  -> residue 8 */
        8,  /* 2^8 mod 19 = 9  -> residue 9 */
        17, /* 2^17 mod 19 = 10-> residue10 */
        12, /* 2^12 mod 19 = 11-> residue11 */
        15, /* 2^15 mod 19 = 12-> residue12 */
        5,  /* 2^5 mod 19 = 13 -> residue13 */
        7,  /* 2^7 mod 19 = 14 -> residue14 */
        11, /* 2^11 mod 19 = 15-> residue15 */
        4,  /* 2^4 mod 19 = 16 -> residue16 */
        10, /* 2^10 mod 19 = 17-> residue17 */
        9   /* 2^9 mod 19 = 18-> residue18 */
    };

    idx_t r = n % 19;            /* residue of n modulo 19 */
    return log_table[r];         /* discrete logarithm, 0 if n ≡ 0 (mod 19) */
}
