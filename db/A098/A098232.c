/*
 * A098232  Largest power of 2 <= 3^n.
 * https://oeis.org/A098232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^floor(n*log₂(3)) (largest power of 2 ≤ 3ⁿ)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   bitlen_seq_t, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"      /* seq_t pow3(idx_t exp);         */
#include "../utils/pow2.h"      /* seq_t pow2(idx_t exp);         */
#include "../utils/bitlen_seq_t.h"  /* unsigned bitlen_seq_t(seq_t); */

seq_t a(idx_t n)
{
    /* Compute 3ⁿ */
    seq_t p = pow3(n);

    /* Find floor(log₂(3ⁿ)) as bit length minus one */
    unsigned bl = bitlen_seq_t(p);
    idx_t   e  = (idx_t)(bl - 1);

    /* Return 2ᵉ, the desired power of two */
    return pow2(e);
}
