/*
 * A053737  Sum of digits of (n written in base 4).
 * https://oeis.org/A053737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = sum of decimal digits of n written in base 4. Recurrence:
 *            a(0) = 0; a(4*n + i) = a(n) + i (0 ≤ i ≤ 3)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sum the base‑4 digits of n.  For an unsigned integer,
       the least significant digit is n & 3 and removing it is
       right‑shifting by 2 bits (divide by 4). */
    seq_t sum = 0;
    idx_t x = n;
    while (x) {
        sum += (seq_t)(x & 3); /* add the current base‑4 digit */
        x >>= 2;               /* drop the processed digit */
    }
    return sum;
}
