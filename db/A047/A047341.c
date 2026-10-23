/*
 * A047341  Numbers that are congruent to {3, 4} mod 7.
 * https://oeis.org/A047341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers congruent to 3 or 4 modulo 7.  
   Formula: for k ≥ 1,
            a(2k-1) = 7k – 4,
            a(2k)   = 7k – 3.
   (Offset 1: a(1)=3, a(2)=4, …) */
seq_t a(idx_t n)
{
    /* n is 1‑based index; input n ≥ 1 */
    if ((n & 1) == 0) {                /* even index: a(2k)   */
        seq_t k = n / 2;               /* k = n/2             */
        return 7 * k - 3;
    } else {                           /* odd index: a(2k-1)  */
        seq_t k = (n + 1) / 2;         /* k = (n+1)/2         */
        return 7 * k - 4;
    }
}
