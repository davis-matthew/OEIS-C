/*
 * A197916  Related to the periodic sequence A171654.
 * https://oeis.org/A197916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=0, a(1)=1, a(2)=6, a(3)=7, a(4)=2,
 * a(n)=a(n-1)+a(n-4)-a(n-5) for all n>=5.
 */
seq_t a(idx_t n)
{
    /* initial terms for n < 5 */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)7;
    if (n == 4) return (seq_t)2;

    /* buffer for the last five values */
    seq_t buf[5] = { (seq_t)0, (seq_t)1, (seq_t)6, (seq_t)7, (seq_t)2 };

    /* compute sequentially up to n */
    for (idx_t i = 5; i <= n; ++i) {
        unsigned idx = (unsigned)(i % 5);          /* i mod 5 */
        seq_t cur = buf[(idx + 4) % 5]            /* a(i-1)  */
                 + buf[(idx + 1) % 5]            /* a(i-4)  */
                 - buf[idx];                    /* -a(i-5) */
        buf[idx] = cur;
    }
    return buf[(unsigned)(n % 5)];
}
