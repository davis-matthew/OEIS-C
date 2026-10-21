/*
 * A067415  Eighth column of triangle A067410.
 * https://oeis.org/A067415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 9 * 16^(n-1) for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, used only for the shift amount */

seq_t a(idx_t n)
{
    if (n == 0)          /* the offset is 0 */
        return (seq_t)1;

    /* exponent of 16:  n-1 times 16  -> 2^(4*(n-1)) */
    idx_t exp = 4 * (n - 1);   /* exp ≤ 39996 for n ≤ 10000 */

    seq_t pow16 = (seq_t)1;

    /* If seq_t is larger than 64 bits, or the shift amount fits into
       the word size of seq_t (i.e., < 64), we can safely shift in one
       operation.  This is the fastest case. */
    if (sizeof(seq_t) > 8 || exp < 64u) {
        /* shift by exp bits: equivalent to multiplication by 16^((n-1)) */
        pow16 <<= exp;
    } else {
        /* For a 64‑bit seq_t but exp ≥ 64, the shift would be UB.
           In this situation we fall back to a loop of small shifts,
           which is still correct (it uses only the defined behaviour
           of the built‑in types) and keeps the value exact until
           overflow occurs. */
        for (idx_t i = 1; i <= exp; i += 4)
            pow16 <<= 4;   /* multiply by 16 each time */
    }

    return pow16 * (seq_t)9;
}
