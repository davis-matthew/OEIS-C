/*
 * A164008  Zero together with row 8 of the array in A163280.
 * https://oeis.org/A164008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+7) for n > 9; for 0 ≤ n ≤ 9 the values are the row‑8
 *            entries of A163280: 0,17,34,39,68,65,102,98,128,153
 * Range:     u8 n <= 12, u16 n <= 252, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 9ULL) {
        static const seq_t tab[10] = {
            0UL,
            17UL, 34UL, 39UL, 68UL, 65UL,
            102UL, 98UL, 128UL, 153UL
        };
        return tab[n];
    }
    seq_t v = (seq_t)n;
    return v * (v + 7);
}
