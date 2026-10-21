/*
 * A096089  Let f(n) = largest number formed using digits of n, g(n) =
 *          smallest number formed using digits of n; then a(n) =
 *          floor(f(n)/g(n)).
 * https://oeis.org/A096089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy, look
 * Formula:   a(n) = floor( f(n) / g(n) ), where f(n) is the number obtained
 *            by arranging the decimal digits of n in non‑increasing order and
 *            g(n) is the number obtained by arranging the digits in
 *            non‑decreasing order (leading zeros are allowed in the
 *            arrangement)
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert the index to a sequence number */
    seq_t x = (seq_t)n;

    /* count decimal digits */
    unsigned cnt[10] = {0};
    while (x > (seq_t)0) {
        unsigned d = (unsigned)(x % (seq_t)10);
        cnt[d]++;
        x /= (seq_t)10;
    }

    /* build f(n) from digits sorted decreasingly */
    seq_t f = (seq_t)0;
    for (int d = 9; d >= 0; --d) {
        for (int i = 0; i < (int)cnt[d]; ++i)
            f = f * (seq_t)10 + (seq_t)d;
    }

    /* build g(n) from digits sorted increasingly */
    seq_t g = (seq_t)0;
    for (int d = 0; d <= 9; ++d) {
        for (int i = 0; i < (int)cnt[d]; ++i)
            g = g * (seq_t)10 + (seq_t)d;
    }

    /* g(n) is never zero for n >= 1, so division is safe */
    return f / g;
}
