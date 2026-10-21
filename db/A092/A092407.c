/*
 * A092407  Coins and notes in the Norwegian currency.
 * https://oeis.org/A092407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1)=1, * a(2)=5; * for n>=3: * if ((n-3)%3<2) a(n)=2*a(n-1);
 *            * else a(n)=(a(n-1)*5)/2
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    seq_t val = (seq_t)5;
    for (idx_t i = 3; i <= n; ++i) {
        int mod = (i - 3) % 3;
        if (mod < 2)
            val = val * 2;
        else
            val = (val * 5) / 2;
    }
    return val;
}
