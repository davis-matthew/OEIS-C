/*
 * A055982  a(n+1) = a(n) converted to base 10 from base 11.
 * https://oeis.org/A055982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n+1) = reinterpret the decimal digits of a(n) as a base‑11
 *            number. Recursive definition: a(0) = 10, a(n+1) = sum_{i=0}^{k}
 *            d_i * 11^i, where d_i are the decimal digits of a(n)
 * Range:     u8 n <= 34, u16 n <= 56, u32 n <= 75, u64 n <= 93, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t cur = (seq_t)10;          /* a(0) */
    if (n == 0) return cur;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t next = 0;             /* a(i) */
        seq_t mult  = 1;            /* 11^position */
        seq_t x = cur;              /* current a(i-1) */
        while (x > 0) {
            seq_t digit = x % (seq_t)10;
            next += digit * mult;
            mult *= (seq_t)11;
            x /= (seq_t)10;
        }
        cur = next;
    }
    return cur;
}
