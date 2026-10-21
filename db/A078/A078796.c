/*
 * A078796  a(n) = 2*ceiling(n*phi) - ceiling(n*sqrt(5)) where
 *          phi=(1+sqrt(5))/2 is the Golden ratio.
 * https://oeis.org/A078796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*ceiling(n*phi) - ceiling(n*sqrt(5))
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where phi = (1+sqrt(5))/2 */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* cast n to seq_t for arithmetic with seq_t values */
    seq_t ns = (seq_t)n;

    /* compute floor(n*sqrt(5)) as floor_sqrt(5*n*n) */
    seq_t n_square = ns * ns;            /* ns^2 */
    seq_t temp     = n_square * (seq_t)5; /* 5*ns^2 */
    seq_t a_root   = floor_sqrt(temp);   /* floor(n*sqrt(5)) */

    /* compute floor(n*phi) */
    seq_t sum     = ns + a_root;         /* ns + floor(n*sqrt(5)) */
    seq_t parity  = sum & (seq_t)1;      /* (ns + a_root) mod 2 */
    seq_t floor_phi = (sum - parity) / (seq_t)2; /* floor(n*phi) */

    /* result = 2*floor_phi - a_root + 1  */
    seq_t result = ((seq_t)2 * floor_phi) - a_root + (seq_t)1;

    return result;
}
