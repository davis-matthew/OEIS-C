/*
 * A054055  Largest digit of n.
 * https://oeis.org/A054055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = max digit of n's decimal representation
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest decimal digit of n. */
seq_t a(idx_t n) {
    if (n == 0u)
        return (seq_t)0;  /* a(0) = 0 */

    seq_t max_digit = (seq_t)0;
    /* n is idx_t (uint64_t), so loop is inexpensive. */
    while (n != 0u) {
        seq_t d = (seq_t)(n % 10u);  /* current least significant decimal digit */
        if (d > max_digit)
            max_digit = d;
        n /= 10u;
    }
    return max_digit;
}
