/*
 * A007456  Number of days required to spread gossip to n people.
 * https://oeis.org/A007456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(1) = 0; a(n) = floor(log_2(n-1)) + ((n-2) mod 2) + 1 for n >=
 *            2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return 0;
    }
    
    idx_t m = n - 1;
    int floorlog2 = 63 - __builtin_clzll(m);
    int parity = (int)((n - 2) & 1);
    
    return (seq_t)(floorlog2 + parity + 1);
}
