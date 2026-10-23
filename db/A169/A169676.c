/*
 * A169676  Lexicographically earliest de Bruijn sequence for n = 2 and k = 3.
 * https://oeis.org/A169676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = period[n mod 9], period[0..8] = {0,0,1,0,2,1,1,2,2} */
seq_t a(idx_t n)
{
    static const seq_t period[9] = {0,0,1,0,2,1,1,2,2};
    int idx = (int)(n % 9ULL);
    return period[idx];
}
