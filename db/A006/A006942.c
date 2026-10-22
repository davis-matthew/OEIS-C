/*
 * A006942  Number of segments used to represent n on calculator display,
 *          variant 5: digits '6', '7' and '9' use 6, 3 and 6 segments,
 *          respectively.
 * https://oeis.org/A006942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, nice, easy
 * Formula:   a(n) = a(floor(n/10)) + a(n mod 10) for n > 9
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const int segments[10] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 6};
    
    if (n == 0) {
        return (seq_t)segments[0];
    }
    
    seq_t total = 0;
    while (n > 0) {
        total += (seq_t)segments[n % 10];
        n /= 10;
    }
    
    return total;
}
