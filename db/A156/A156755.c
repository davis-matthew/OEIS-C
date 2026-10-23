/*
 * A156755  Period 9: repeat [1, 1, 2, 1, 1, 2, 2, 2, 3].
 * https://oeis.org/A156755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=1, a(4)=1, a(5)=2, a(6)=2, a(7)=2,
 *            a(8)=3 Then a(n)=a(n-9) for all n ≥ 9
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 9;   /* period 9 */
    switch (r) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)1;
        case 2: return (seq_t)2;
        case 3: return (seq_t)1;
        case 4: return (seq_t)1;
        case 5: return (seq_t)2;
        case 6: return (seq_t)2;
        case 7: return (seq_t)2;
        case 8: return (seq_t)3;
        default: return (seq_t)0; /* unreachable */
    }
}
