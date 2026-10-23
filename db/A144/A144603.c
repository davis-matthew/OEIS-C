/*
 * A144603  Christoffel word of slope 5/11.
 * https://oeis.org/A144603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-16). The sequence is 16‑periodic. For 0 ≤ n < 16,
 *            a(n) = 1 if n∈{3,6,9,12,15}, otherwise 0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n modulo 16 gives the position within one period */
    idx_t m = n % 16;
    /* a(n) is 1 only for 3,6,9,12,15 → m % 3 == 0 but m != 0 */
    return (seq_t)((m % 3 == 0 && m != 0) ? 1 : 0);
}
