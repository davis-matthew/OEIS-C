/*
 * A023436  Dying rabbits: a(n) = a(n-1) + a(n-2) - a(n-6).
 * https://oeis.org/A023436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-6) with a(0)=0, a(1)=1, a(2)=1,
 *            a(3)=2, a(4)=3, a(5)=5
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 52, u64 n <= 104,
 *            u128 n <= 208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)3;
    if (n == 5) return (seq_t)5;

    seq_t prev6 = (seq_t)0;
    seq_t prev5 = (seq_t)1;
    seq_t prev4 = (seq_t)1;
    seq_t prev3 = (seq_t)2;
    seq_t prev2 = (seq_t)3;
    seq_t prev1 = (seq_t)5;
    seq_t cur   = 0;

    for (idx_t i = 6; i <= n; ++i) {
        cur = prev1 + prev2 - prev6;
        prev6 = prev5;
        prev5 = prev4;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
