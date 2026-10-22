/*
 * A001754  Lah numbers: a(n) = n!*binomial(n-1,2)/6.
 * https://oeis.org/A001754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*binomial(n-1,2)/6 = (Product_{i=4..n} i) *
 *            binomial(n-1,2), a(1) = a(2) = 0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3) return 0;

    seq_t result = 1;
    idx_t i;
    for (i = 4; i <= n; i++)
        result *= (seq_t)i;

    seq_t b;
    if ((n - 1) % 2 == 0)
        b = (seq_t)((n - 1) / 2) * (seq_t)(n - 2);
    else
        b = (seq_t)(n - 1) * (seq_t)((n - 2) / 2);

    return result * b;
}
