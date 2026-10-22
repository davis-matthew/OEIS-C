/*
 * A007284  Horizontally symmetric numbers.
 * https://oeis.org/A007284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   write n in base 4, then map digits 0,1,2,3 -> 0,1,3,8 and read
 *            the result in base 10
 * Range:     u8 n <= 31, u16 n <= 767, u32 n <= 9258, u64 n <= 9258,
 *            u128 n <= 9258, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const int digit_map[4] = {0, 1, 3, 8};
    seq_t result = 0;
    seq_t power = 1;
    idx_t temp = n;

    while (temp > 0) {
        int d = digit_map[(int)(temp % 4)];
        if (d != 0)
            result += power * d;
        power *= 10;
        temp /= 4;
    }

    return result;
}
