/*
 * A026636  a(n) = Sum_{k=0..floor(n/2)} A026626(n-k, k).
 * https://oeis.org/A026636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-4) - a(n-5) - a(n-6) + b(n), where
 *            b(0)=1, b(3)=1, b(4)=-1, b(5)=1, b(7)=1, and b(n)=0 otherwise.
 *            Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=6, a(5)=11
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[6] = { (seq_t)1,
                                   (seq_t)1,
                                   (seq_t)2,
                                   (seq_t)4,
                                   (seq_t)6,
                                   (seq_t)11 };
    if (n <= 5) return base[n];

    seq_t buf[6];
    for (idx_t i = 0; i < 6; ++i) buf[i] = base[i];

    for (idx_t i = 6; i <= n; ++i) {
        seq_t b = (i == 7) ? (seq_t)1 : (seq_t)0;
        seq_t val = buf[5] + buf[4] + buf[2]; /* a(n‑1)+a(n‑2)+a(n‑4) */
        val -= buf[1];                        /* -a(n‑5)            */
        val -= buf[0];                        /* -a(n‑6)            */
        val += b;

        /* shift buffer left by one: buf[0] becomes old buf[1], …     */
        for (idx_t j = 0; j < 5; ++j) buf[j] = buf[j + 1];
        buf[5] = val;
    }
    return buf[5];
}
