/*
 * A201208  One 1, two 2's, three 1's, four 2's, five 1's, ...
 * https://oeis.org/A201208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 if the smallest k with k(k+1)/2 ≥ n is odd, * a(n) = 2
 *            if that k is even. Equivalent: a(n)=A057212(n)+1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find the smallest k such that k(k+1)/2 >= n */
    idx_t k = 0;
    idx_t tri = 0;
    while (tri < n) {
        ++k;
        tri += k;
    }
    /* k is the group index: odd → 1, even → 2 */
    return (k % 2 == 0) ? (seq_t)2 : (seq_t)1;
}
