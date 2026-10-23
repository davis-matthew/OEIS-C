/*
 * A101502  Number of closed walks on C_5 tensor J_2.
 * https://oeis.org/A101502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 12*a(n-2) - 16*a(n-3), for n > 4 Initial
 *            values: a(0)=1, a(1)=0, a(2)=4, a(3)=0, a(4)=48
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[5] = { (seq_t)1, (seq_t)0, (seq_t)4, (seq_t)0, (seq_t)48 };
    if (n < 5) return base[n];

    seq_t a0 = base[2];   /* a(n-3) for n = 5 */
    seq_t a1 = base[3];   /* a(n-2) for n = 5 */
    seq_t a2 = base[4];   /* a(n-1) for n = 5 */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t curr = 2 * a2 + 12 * a1;
        curr -= 16 * a0;
        a0 = a1;
        a1 = a2;
        a2 = curr;
    }
    return a2;
}
