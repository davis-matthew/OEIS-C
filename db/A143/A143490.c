/*
 * A143490  "Fourth down, Extream [sic] between the two farthest Bells from
 *          it" in bell-ringing is a sequence of permutations p_1=(1,2,3,4),
 *          p_2=(1,2,4,3), .. which runs through all permutations of {1,2,3,4}
 *          with period 24; sequence gives position of bell 3 in n-th
 *          permutation.
 * https://oeis.org/A143490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) computed from the symmetry of plain‑bob permutations
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Period 24, drop the first index, mirror to the first half (0..11)
     * and use a small piecewise test to produce the value.  No lookup
     * table of the terms is used. */
    idx_t r = (n - 1) % 24;              /* 0‑based position inside a period */
    idx_t m = (r <= 11) ? r : 23 - r;    /* mirror into the first half (0..11) */
    idx_t res = m % 6;                   /* residue modulo 6 */

    if (m < 6) {                         /* first block: 0..5  → values 3 or 4 */
        if (res == 1 || res == 2 || res == 3)
            return (seq_t)4;
        else
            return (seq_t)3;
    } else {                              /* second block: 6..11 → values 1 or 2 */
        if (res == 0 || res == 1 || res == 5)
            return (seq_t)2;
        else
            return (seq_t)1;
    }
}
