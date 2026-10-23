/*
 * A028765  Nonsquares mod 52.
 * https://oeis.org/A028765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the n-th positive integer that is NOT a quadratic
 * residue modulo 52.
 * It is obtained by enumerating the 38 nonsquare residues modulo 52
 * and taking the n-th one in increasing order.
 */

#include <stddef.h>

/* Squares modulo 13: 0,1,3,4,9,10,12. */
static const unsigned char squares13[13] = {
    1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1
};

/* true if x is a quadratic residue modulo 52 */
static int is_square_mod52(unsigned int x)
{
    unsigned int r4 = x & 3;          /* x mod 4 */
    if (r4 > 1) return 0;             /* only 0 or 1 can be squares mod 4 */
    return squares13[x % 13];
}

/* the idx‑th nonsquare residue modulo 52 (idx: 0‑based) */
static unsigned int nonsquare_residue(idx_t idx)
{
    idx_t count = 0;
    for (unsigned int i = 0; i < 52; ++i) {
        if (!is_square_mod52(i)) {
            if (count == idx) return i;
            ++count;
        }
    }
    return 0;  /* should never occur for valid idx */
}

seq_t a(idx_t n)
{
    /* number of complete blocks of 38 nonsquares */
    idx_t block = (n - 1) / 38;
    /* index within the current block */
    idx_t rem   = (n - 1) % 38;
    /* base value contributed by full blocks (52 per block) */
    seq_t base  = (seq_t)block * (seq_t)52;
    return base + (seq_t)nonsquare_residue(rem);
}
