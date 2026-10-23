/*
 * A158677  Period 6: repeat [3, 4, 0, 5, 6, 3].
 * https://oeis.org/A158677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6) for n>6 with a(1)=3, a(2)=4, a(3)=0, a(4)=5,
 *            a(5)=6, a(6)=3
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = (n - 1) % 6;      /* n starts at 1 */
    switch (r) {
        case 0: return (seq_t)3;   /* n ≡ 1 (mod 6) */
        case 1: return (seq_t)4;   /* n ≡ 2 (mod 6) */
        case 2: return (seq_t)0;   /* n ≡ 3 (mod 6) */
        case 3: return (seq_t)5;   /* n ≡ 4 (mod 6) */
        case 4: return (seq_t)6;   /* n ≡ 5 (mod 6) */
        case 5: return (seq_t)3;   /* n ≡ 0 (mod 6) */
        default: return (seq_t)0;  /* unreachable */
    }
}
