/*
 * A101661  Fixed point of morphism 0 -> 01, 1 -> 02, 2 -> 20.
 * https://oeis.org/A101661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n)=2[a(n)==2], a(2n+1)=(a(n)+1) mod 3
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    if ((n & 1ULL) == 0) {                    /* n is even: n = 2k */
        seq_t t = a(n >> 1);                  /* a(k) */
        return (t == 2 ? 2 : 0);
    } else {                                  /* n is odd: n = 2k+1 */
        seq_t t = a(n >> 1);                  /* a(k) */
        return (t + 1) % 3;
    }
}
