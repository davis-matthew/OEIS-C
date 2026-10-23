/*
 * A041202  Numerators of continued fraction convergents to sqrt(112).
 * https://oeis.org/A041202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 254*a(n-6) - a(n-12) for n >= 12, with initial values
 *            a(0)=10, a(1)=11, a(2)=21, a(3)=53, a(4)=74, a(5)=127,
 *            a(6)=2614, a(7)=2741, a(8)=5355, a(9)=13451, a(10)=18806,
 *            a(11)=32257
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial twelve terms */
    static const seq_t init[12] = {
        (seq_t)10, (seq_t)11, (seq_t)21, (seq_t)53,
        (seq_t)74, (seq_t)127, (seq_t)2614, (seq_t)2741,
        (seq_t)5355, (seq_t)13451, (seq_t)18806, (seq_t)32257
    };

    if (n < 12)
        return init[n];

    /* ring buffer of the most recent 12 terms */
    seq_t buf[12];
    for (idx_t i = 0; i < 12; ++i)
        buf[i] = init[i];

    for (idx_t i = 12; i <= n; ++i) {
        /* indices modulo 12 refer to the buffer positions of a(i-6)
           and a(i-12) which have not yet been overwritten in this
           iteration */
        seq_t val = (seq_t)254 * buf[(i - 6) % 12] - buf[(i - 12) % 12];
        buf[i % 12] = val;
    }

    return buf[n % 12];
}
