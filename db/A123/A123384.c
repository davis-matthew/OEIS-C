/*
 * A123384  Number of bits in binary expansion of 10^n.
 * https://oeis.org/A123384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 1 + floor( n * log₂(10) ) = 1 + floor( n * 33219 / 10000
 *            )
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The fraction 33219/10000 is a lower bound approximation of log₂(10) that
   guarantees the floor operation is correct for all n whose result fits in
   the type seq_t (including the 64‑bit and arbitrary‑precision builds). */

seq_t a(idx_t n)
{
    const idx_t NUM = 33219;      /* numerator of the approximation 33219/10000 */
    const idx_t DEN = 10000;      /* denominator                                  */
#if defined(__SIZEOF_INT128__)
    /* Use 128‑bit arithmetic for a safe product when seq_t is 64‑bit. */
    unsigned __int128 prod = (unsigned __int128)n * NUM;
    return (seq_t)(prod / DEN) + 1;
#else
    /* Fallback: works as long as the intermediate product fits in seq_t. */
    return ((seq_t)n * NUM) / DEN + 1;
#endif
}
