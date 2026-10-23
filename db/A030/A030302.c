/*
 * A030302  Write n in base 2 and juxtapose; irregular table in which row n
 *          lists the binary expansion of n.
 * https://oeis.org/A030302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy, tabf
 * Formula:   the nth term is the nth bit in the binary Champernowne constant
 * Range:     u8 n <= 2054, u16 n <= 3998, u32 n <= 3998, u64 n <= 3998,
 *            u128 n <= 3998, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for standard integer types if needed */

/* number of bits needed to represent x (x >= 1) */
static unsigned bitlen_idx(idx_t x)
{
    unsigned len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len; /* x is at least 1 by construction */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 0 is outside the defined range */

    idx_t target = n;                /* position we want, 1‑based */
    idx_t cum = 0;                   /* cumulative number of bits processed so far */
    idx_t k = 1;                     /* current integer being written in binary */
    unsigned len;

    /* Find the integer k whose binary representation contains the target bit */
    while (1) {
        len = bitlen_idx(k);
        if (target - cum <= len) break;
        cum += len;
        ++k;
    }

    /* offset within k's binary representation (1‑based) */
    unsigned offset = target - cum;
    unsigned shift  = len - offset;    /* position of the desired bit from the LSB */
    seq_t bit = ((seq_t)k >> shift) & 1;
    return bit;
}
