/*
 * A044950  Number of runs of odd length in the base 10 representation of n.
 * https://oeis.org/A044950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of runs of odd length in the decimal
 *            representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;               // Undefined for 0, return 0
    idx_t nn = n;
    unsigned char prev = 255;           // sentinel for no previous digit
    unsigned char len = 0;
    seq_t count = 0;

    while (nn > 0) {
        unsigned char digit = nn % 10;
        if (digit == prev) {
            ++len;
        } else {
            if (prev != 255 && (len & 1)) {
                ++count;
            }
            prev = digit;
            len = 1;
        }
        nn /= 10;
    }

    if (len & 1) {                      // process last run
        ++count;
    }
    return count;
}
