/*
 * A080010  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=2, r=3, I={1}.
 * https://oeis.org/A080010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=a(n-1)+a(n-2)+2*a(n-3)+3*a(n-4)+3*a(n-5)-2*a(n-6)-a(n-7)-a(n-10);
 *            initial terms: a(0)=1, a(1)=1, a(2)=1, a(3)=3, a(4)=9, a(5)=19,
 *            a(6)=38, a(7)=84, a(8)=193, a(9)=430, a(10)=940
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first 11 terms */
    static const seq_t init[11] = {
        1, 1, 1, 3, 9, 19, 38, 84, 193, 430, 940
    };

    if (n < 11)        /* return known value directly */
        return init[n];

    /* a circular buffer that always contains the last 11 values */
    seq_t buf[11];
    for (int i = 0; i < 11; ++i)
        buf[i] = init[i];

    for (idx_t i = 11; i <= n; ++i)
    {
        idx_t idx = i % 11;

        seq_t val = buf[(i-1) % 11] + buf[(i-2) % 11];
        val += 2 * buf[(i-3) % 11];
        val += 3 * buf[(i-4) % 11];
        val += 3 * buf[(i-5) % 11];
        val -= 2 * buf[(i-6) % 11];
        val -=     buf[(i-7) % 11];
        val -=     buf[(i-10) % 11];

        buf[idx] = val;
    }

    return buf[n % 11];
}
