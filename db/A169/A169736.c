/*
 * A169736  First differences of A169735.
 * https://oeis.org/A169736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if n==1; 18 if n==2; 9 if 3≤n≤11; 0 otherwise
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)18;
    if (n >= 3 && n <= 11) return (seq_t)9;
    return (seq_t)0;
}
