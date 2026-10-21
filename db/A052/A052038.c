/*
 * A052038  First nonzero digit in the decimal expansion of 1/n.
 * https://oeis.org/A052038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = first non‑zero digit of the decimal expansion of 1/n
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == (idx_t)1)
        return (seq_t)1;          /* 1/1 = 1.0, first non‑zero digit is 1 */
    seq_t remainder = (seq_t)1;
    /* Long division: keep multiplying by 10 until a non‑zero digit appears */
    while (1) {
        remainder *= (seq_t)10;
        seq_t digit = remainder / (seq_t)n;      /* digit 0‑9 */
        remainder %= (seq_t)n;
        if (digit != (seq_t)0)
            return digit;
    }
}
