/*
 * A160108  Lodumo_8 of Fibonacci numbers.
 * https://oeis.org/A160108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=9, a(3)=2, a(4)=3, a(5)=5, a(6)=8, a(7)=13,
 *            a(8)=21, a(9)=10, a(10)=7, a(11)=17; for n≥12, a(n) = 2*a(n-6) –
 *            a(n-12)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _a0_11[12] = {0, 1, 9, 2, 3, 5, 8, 13, 21, 10, 7, 17};

seq_t a(idx_t n)
{
    if (n < 12) return _a0_11[n];

    /* Buffer holding the last 12 computed values.
       buf[i] always contains a(i) modulo 12. */
    seq_t buf[12];
    for (int i = 0; i < 12; ++i) buf[i] = _a0_11[i];

    for (idx_t i = 12; i <= n; ++i)
    {
        seq_t two_prev6 = buf[(i - 6) % 12] + buf[(i - 6) % 12];
        seq_t val = two_prev6;
        val -= buf[(i - 12) % 12];
        buf[i % 12] = val;
    }

    return buf[n % 12];
}
