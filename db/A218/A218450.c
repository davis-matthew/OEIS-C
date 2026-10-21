/*
 * A218450  Number of digits of n plus number of digits of n equal to 1, 2, 4,
 *          or 8.
 * https://oeis.org/A218450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of decimal digits of n + number of those digits
 *            that are one of {1,2,4,8}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return 0; /* offset starts at 1, guard for completeness */
    idx_t temp = n;
    seq_t digitCount = 0;
    seq_t setCount = 0;
    while (temp > 0) {
        ++digitCount;
        switch (temp % 10) {
            case 1:
            case 2:
            case 4:
            case 8:
                ++setCount;
                break;
        }
        temp /= 10;
    }
    return digitCount + setCount;
}
