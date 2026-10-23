/*
 * A135513  Number of Pierce-Engel hybrid expansions of 4/b, b>=4.
 * https://oeis.org/A135513
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence: A135513, a(n) = a(n mod 12) for every n ≥ 4  
   (period 12), with the pattern
   4→1, 5→4, 6→2, 7→4, 8→1, 9→2, 10→2, 11→4, 12→1, 13→4, 14→2, 15→2. */

seq_t a(idx_t n)
{
    static const seq_t table[12] = {1U, 4U, 2U, 2U, 1U, 4U, 2U, 4U, 1U, 2U, 2U, 4U};
    return table[(unsigned)(n % 12)];
}
