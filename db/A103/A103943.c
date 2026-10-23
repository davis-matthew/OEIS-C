/*
 * A103943  Number of unrooted two-vertex n-edge maps in the plane (planar
 *          with a distinguished outside face).
 * https://oeis.org/A103943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^(2n-1) - C(2n-1,n-1) + C(n-1,⌊n/2⌋)) / 2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   binom_big_u128, binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"
#include "../utils/binom_big_u128.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1 */

    /* Separate the 64‑bit and arbitrary‑precision cases. */
    if (sizeof(seq_t) <= 8) {
        /* All arithmetic done in 128‑bit integers to avoid overflow. */
        /* 2^(2n-1) safely fits in 128‑bit for n ≤ 64.  For larger n the
           result does not fit in 64‑bit and the 64‑bit build is not
           expected to be queried. */
        if (2 * n - 1 >= 128) return 0;   /* safety guard */

        unsigned __int128 pow2val = ((unsigned __int128)1) << (2 * n - 1);
        unsigned __int128 c1 = binom_big_u128(2 * n - 1, n - 1);
        unsigned __int128 c2 = binom_big_u128(n - 1, n / 2);
        unsigned __int128 value = (pow2val - c1 + c2) >> 1; /* divide by 2 */
        return (seq_t)value;
    } else {
        /* Arbitrary‑precision build: use utility routines that can
           handle arbitrarily large intermediate values. */
        seq_t pow2val   = pow2(2 * n - 1);
        seq_t c1        = binomial(2 * n - 1, n - 1);
        seq_t c2        = binomial(n - 1, n / 2);
        seq_t temp = pow2val - c1 + c2;
        return temp / 2;
    }
}
