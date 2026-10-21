/*
 * A070342  a(n) = 3^n mod 19.
 * https://oeis.org/A070342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 19
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t table[18];
    static int init = 0;
    if (!init) {
        table[0] = 1;  /* 3^0 mod 19 */
        for (int i = 1; i < 18; ++i) {
            table[i] = (table[i - 1] * (seq_t)3) % (seq_t)19;
        }
        init = 1;
    }
    return table[n % (idx_t)18];
}
