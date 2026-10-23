/*
 * A118673  Positive solutions x to the equation x^2 + (x+71)^2 = y^2.
 * https://oeis.org/A118673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=13, a(2)=160, a(3)=213, a(4)=280, a(5)=1113. For n
 *            ≥ 6: a(n) = 6*a(n‑3) – a(n‑6) + 142
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 31, u64 n <= 69, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[6] = { (seq_t)0, (seq_t)13, (seq_t)160,
                                   (seq_t)213, (seq_t)280, (seq_t)1113 };

    /* Return initial values directly. */
    if (n < 6) return init[n];

    /* ring buffer of the last six computed terms */
    seq_t buf[6];
    for (idx_t i = 0; i < 6; ++i) buf[i] = init[i];

    for (idx_t i = 6; i <= n; ++i) {
        seq_t val = ( (seq_t)6 * buf[(i - 3) % 6] )
                    - buf[(i - 6) % 6]
                    + (seq_t)142;
        buf[i % 6] = val;
    }

    return buf[n % 6];
}
