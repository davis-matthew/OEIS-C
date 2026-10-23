/*
 * A037773  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1,0,2.
 * https://oeis.org/A037773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A037773: Base 5 digits are, in order, the first n terms of
   the periodic sequence with initial period 3,1,0,2.
   a(n) = value of the base‑5 number whose digits are
   d_1,d_2,…,d_n where d_k = {3,1,0,2}[ (k-1) mod 4 ].
   The recurrence is a(1)=3, a(n)=a(n-1)*5+d_n for n>1. */
seq_t a(idx_t n) {
    static const unsigned char cycle[4] = {3,1,0,2};
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i) {
        res = res * 5 + cycle[(i-1) & 3U];  /* (i-1) % 4 but faster */
    }
    return res;
}
