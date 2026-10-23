/*
 * A126944  The sequence a[n]+b[n]+c[n]+d[n] defined in A126939.
 * https://oeis.org/A126944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+a(n-2)+7*a(n-3)+3*a(n-4)+3*a(n-5)+a(n-6) for n
 *            >= 6, with initial values a(0)=1, a(1)=5, a(2)=15, a(3)=47,
 *            a(4)=149, a(5)=469
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[6] = {1,5,15,47,149,469};
    if (n < 6) return init[n];
    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t a5 = init[5];
    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = 2*a5 + a4 + 7*a3 + 3*a2 + 3*a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
