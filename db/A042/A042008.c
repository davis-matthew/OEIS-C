/*
 * A042008  Numerators of continued fraction convergents to sqrt(527).
 * https://oeis.org/A042008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 1056*a(n-4) - a(n-8) Initial values: a(0)=22, a(1)=23,
 *            a(2)=505, a(3)=528, a(4)=23737, a(5)=24265, a(6)=533302,
 *            a(7)=557567
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[8] = {
        (seq_t)22,
        (seq_t)23,
        (seq_t)505,
        (seq_t)528,
        (seq_t)23737,
        (seq_t)24265,
        (seq_t)533302,
        (seq_t)557567
    };
    if (n < 8)
        return init[n];

    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    seq_t next;
    for (idx_t i = 8; i <= n; ++i) {
        next = (seq_t)1056 * buf[(i - 4) & 7] - buf[(i - 8) & 7];
        buf[i & 7] = next;
    }

    return buf[n & 7];
}
