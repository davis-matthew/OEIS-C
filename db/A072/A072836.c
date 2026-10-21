/*
 * A072836  Exponents occurring in expansion of F_10(q^2).
 * https://oeis.org/A072836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*floor((n-1)/6)+k, where k = {9,16,20,21,24,25} for n
 *            mod 6 = {1,2,3,4,5,0} respectively, and a(0)=0
 * Range:     u8 n <= 73, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Predefined remainder-to-offset mapping for n % 6 */
    static const int rem_offset[6] = {25, 9, 16, 20, 21, 24};

    idx_t block = (n - 1) / 6;          /* floor((n-1)/6)   */
    int remainder = n % 6;              /* n mod 6          */
    seq_t base = (seq_t)20 * (seq_t)block;  /* 20 * block      */

    return base + (seq_t)rem_offset[remainder];
}
