/*
 * A117798  Icosagonal numbers divisible by 20.
 * https://oeis.org/A117798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 4370, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = i*(9i-8),  where i = 20*floor((n-1)/4) + {0,2,10,12}[(n-1) mod 4] */
/* icosagonal numbers that are divisible by 20 (first term 0 for n=1) */

seq_t a(idx_t n)
{
    /* n >= 1  is guaranteed */
    idx_t k = n - 1;               /* zero‑based rank */
    idx_t q = k / 4;               /* number of full 20‑blocks */
    idx_t r = k % 4;               /* position inside the block */
    static const idx_t offs[4] = {0, 2, 10, 12};
    idx_t i = 20 * q + offs[r];    /* the index of the icosagonal number */

    /* compute icosagonal number i*(9i-8) in seq_t arithmetic */
    seq_t ii = (seq_t)i;
    seq_t val = ii * ((seq_t)9 * ii - 8);
    return val;
}
