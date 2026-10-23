/*
 * A010167  Continued fraction for sqrt(96).
 * https://oeis.org/A010167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 9; for n >= 1, a(n) = 1 if n is odd, a(n) = 3 if n ≡ 2
 *            (mod 4), a(n) = 18 if n ≡ 0 (mod 4)
 * Range:     u8 n <= 9469, u16 n <= 9469, u32 n <= 9469, u64 n <= 9469,
 *            u128 n <= 9469, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)9;
    /* (n-1) mod 4 can be obtained with bitwise AND, since 4 is a power of 2 */
    idx_t r = (n - 1) & 3u; /* r ∈ {0,1,2,3} */

    switch (r)
    {
        case 0: return (seq_t)1;   /* n ≡ 1 (mod 4): odd */
        case 1: return (seq_t)3;   /* n ≡ 2 (mod 4): 2 · odd */
        case 2: return (seq_t)1;   /* n ≡ 3 (mod 4): odd */
        default: return (seq_t)18; /* r == 3: n ≡ 0 (mod 4): divisible by 4 */
    }
}
