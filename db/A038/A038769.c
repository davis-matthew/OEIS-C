/*
 * A038769  Number of digits of n which are divisors of n.
 * https://oeis.org/A038769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = |{d in decimal digits of n : d != 0 and n mod d == 0}|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t orig = n;
    seq_t cnt = 0;
    while (n > 0) {
        idx_t d = n % 10;            /* current decimal digit */
        if (d != 0 && orig % d == 0)
            cnt++;
        n = n / 10;
    }
    return cnt;
}
