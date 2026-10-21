/*
 * A059765  Possible sizes of the torsion group of an elliptic curve over the
 *          rationals Q. This is a finite sequence.
 * https://oeis.org/A059765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = {1,2,3,4,5,6,7,8,9,10,12,16} for 1 ≤ n ≤ 12,
 *             0 for n > 12.
 * The sequence lists the possible orders of the torsion subgroup of
 * an elliptic curve over the rationals (Mazur's theorem).
 */
seq_t a(idx_t n)
{
    static const seq_t vals[12] = {
        (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)4,
        (seq_t)5, (seq_t)6, (seq_t)7, (seq_t)8,
        (seq_t)9, (seq_t)10, (seq_t)12, (seq_t)16
    };

    if (n >= 1 && n <= 12) {
        /* n-1 fits into size_t because n ≤ 12 */
        return vals[n - 1];
    }
    return (seq_t)0;
}
