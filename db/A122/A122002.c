/*
 * A122002  a(0)=5; otherwise a(n) = (n mod 4) if n is odd, a(n) = h + 4,
 *          where h = (highest odd divisor of n) mod 4 if n is even.
 * https://oeis.org/A122002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=5; for n≥1, if n is odd: a(n)=n mod 4; if n is even:
 *            a(n)=((highest odd divisor of n) mod 4)+4
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)5;
    if (n & 1) {                /* n is odd */
        return (seq_t)(n & 3);  /* n mod 4 (1 or 3) */
    } else {                    /* n is even */
        idx_t h = n;
        while ((h & 1) == 0)    /* strip factors of 2 */
            h >>= 1;
        return (seq_t)(h & 3) + 4; /* (h mod 4) + 4 (5 or 7) */
    }
}
