/*
 * A037758  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,1,2.
 * https://oeis.org/A037758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = sum_{k=1..n} d_k * 4^{n-k},
   where d_k = 3,0,1,2 repeating (d_1=3, d_2=0, d_3=1, d_4=2).
   Equivalently, a(1)=3, a(n)=4*a(n-1)+d_n. */
seq_t a(idx_t n)
{
    seq_t result = 0;
    /* The repeating 4‑digit pattern */
    static const unsigned char pat[4] = {3, 0, 1, 2};
    for (idx_t i = 1; i <= n; ++i) {
        /* multiply by 4 and add the next digit of the pattern */
        result = result * ((seq_t)4) + (seq_t)pat[(i - 1) & 3];
    }
    return result;
}
