/*
 * A101673  Fixed point of morphism 0 -> 01, 1 -> 20, 2 -> 20.
 * https://oeis.org/A101673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth symbol of the fixed point of the morphism 0 →
 *            01, 1 → 20, 2 → 20. The sequence starts 0,1,2,0,2,0,0,1,
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* base symbol */
    seq_t prev = a(n >> 1);                 /* symbol at the parent level */
    return (prev == 0) ?                /* from 0 → 01 */
           ((n & 1) ? (seq_t)1 : (seq_t)0)   /* even → 0, odd → 1 */
           :                              /* from 1 or 2 → 20 */
             ((n & 1) ? (seq_t)0 : (seq_t)2); /* even → 2, odd → 0 */
}
