/*
 * A162818  Strictly positive numbers n such that (42*n)/(42+n) is an integer.
 * https://oeis.org/A162818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 1764/d – 42 where d·42 – 42 = other terms, i.e.
 *            (42·n)/(42+n) is an integer. The sequence is finite; it consists
 *            of the 13 values obtained by iterating the divisor d of 1764
 *            (42²) from 41 down to 1 and computing n = 1764/d – 42
 * Range:     u8 none, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Static storage for the 13 terms.  Filled on first call. */
    static seq_t table[13];
    static idx_t count = 0;

    if (count == 0) {
        const seq_t M = (seq_t)42 * (seq_t)42;   /* 1764 */
        const seq_t sub = (seq_t)42;
        idx_t d = 41;
        idx_t pos = 0;

        while (d > 0) {
            const seq_t dd = (seq_t)d;
            /* Keep d only if it divides 1764. */
            if ((M % dd) == 0) {
                table[pos++] = M / dd - sub;
            }
            d--;
        }
        count = pos;   /* should be 13 */
    }

    if (n == 0 || n > count) {
        return (seq_t)0;
    }
    return table[n - 1];
}
