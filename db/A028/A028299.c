/*
 * A028299  Weight distribution of the dodecacode, a [[12,0,6]]
 *          quantum-error-correcting code.
 * https://oeis.org/A028299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Weight distribution of the dodecacode, a [[12,0,6]] quantum-error-correcting code.
   a(0) = 1
   a(1) = 0
   a(2) = 0
   a(3) = 396
   a(4) = 1485
   a(5) = 1980
   a(6) = 234
   All other a(n) = 0. */
seq_t a(idx_t n)
{
    /* The distribution has non‑zero values only for 0 ≤ n ≤ 6. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)396;
    if (n == 4) return (seq_t)1485;
    if (n == 5) return (seq_t)1980;
    if (n == 6) return (seq_t)234;
    return (seq_t)0;
}
