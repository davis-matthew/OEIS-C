/*
 * A204032  Number of (n+1) X 2 0..1 arrays with the sums of 2 X 2 subblocks
 *          nondecreasing rightwards and downwards.
 * https://oeis.org/A204032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204032
 * Closed form:
 *   let Lₑ = floor((n+2)/2),   Lₒ = floor((n+1)/2)
 *   S(k) = 2^(k+2) – (k+3)
 *   a(n) = S(Lₑ) * S(Lₒ)
 * the values for n ≥ 1 match the OEIS b-file.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute 2^e as seq_t using shifts – works for all seq_t types. */
static seq_t pow2_shift(idx_t e)
{
    seq_t val = (seq_t)1;
    for (idx_t i = 0; i < e; ++i)
        val <<= 1u;
    return val;
}

/* public interface */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset in OEIS is 1 – undefined for 0 */
        return (seq_t)0;

    /* lengths of even‐indexed and odd‐indexed rows */
    idx_t Le = (n + 2) / 2;   /* ceil((n+2)/2)  */
    idx_t Lo = (n + 1) / 2;   /* floor((n+1)/2) */

    /* S(k) = 2^(k+2) – (k+3) */
    seq_t Se = pow2_shift(Le + 2) - (seq_t)(Le + 3);
    seq_t So = pow2_shift(Lo + 2) - (seq_t)(Lo + 3);

    return Se * So;
}
