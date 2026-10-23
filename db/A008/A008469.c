/*
 * A008469  a(n) = n OR n^3 (applied to ternary expansions).
 * https://oeis.org/A008469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = digitwise OR of n and n³ in base 3, indexed from 1
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nt  = (seq_t)n;           /* n */
    seq_t n3  = nt * nt * nt;       /* n³ */

    seq_t result = 0;
    seq_t power  = 1;               /* 3^position, starts at 3⁰ */

    while (nt != 0 || n3 != 0) {
        seq_t d1 = nt % 3;          /* digit of n */
        seq_t d2 = n3 % 3;          /* digit of n³ */
        seq_t dm = d1 > d2 ? d1 : d2; /* OR == max for base‑3 digits */

        result += dm * power;       /* add the digit at the current position */
        power *= 3;                 /* advance to the next power of 3 */

        nt  /= 3;
        n3  /= 3;
    }
    return result;
}
