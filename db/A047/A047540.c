/*
 * A047540  Numbers that are congruent to {0, 2, 4, 7} mod 8.
 * https://oeis.org/A047540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + [0,2,4,7][ (n-1) mod 4 ]
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A047540 (numbers ≡ 0,2,4,7 (mod 8)). */
seq_t a(idx_t n)
{
    /* Guard against n < 1: undefined behaviour for such inputs.   */
    /* For n ≥ 1 we compute as described above.                     */
    static const seq_t rem[4] = { (seq_t)0, (seq_t)2, (seq_t)4, (seq_t)7 };

    /* Adjust to get zero‑based index for the pattern.              */
    idx_t k = n - 1;                     /* k ≥ 0 */
    idx_t q = k / 4;                     /* integer quotient */
    idx_t r = k % 4;                     /* remainder in [0,3] */

    /* Perform multiplication in seq_t domain.                        */
    seq_t val = (seq_t)8 * (seq_t)q;      /* 8*q  */
    val += rem[r];                       /* add the appropriate remainder */

    return val;
}
