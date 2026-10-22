/*
 * A001729  List of numbers whose digits contain no loops (version 1).
 * https://oeis.org/A001729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) is n written in bijective base 6, with the bijective digits
 *            1..6 mapped to the decimal digits 1,2,3,4,5,7
 * Range:     u8 n <= 107, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const int digits[6] = {1, 2, 3, 4, 5, 7};
    seq_t result = 0;
    seq_t multiplier = 1;

    while (n > 0) {
        idx_t r = (n - 1) % 6;
        result += multiplier * digits[r];
        n = (n - 1) / 6;
        multiplier *= 10;
    }

    return result;
}
