/*
 * A037741  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,3,0.
 * https://oeis.org/A037741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = value whose base‑8 digits are the first n terms of the
 *            periodic * sequence 2,1,3,0 repeated. That is, a(0)=0 and * a(k)
 *            = 8*a(k‑1) + d_k for k ≥ 1, where d_k = {2,1,3,0}[ (k-1) mod 4 ]
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const unsigned char pat[4] = { 2, 1, 3, 0 };
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i) {
        res = res * ((seq_t)8) + (seq_t)pat[i & 3];
    }
    return res;
}
