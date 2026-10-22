/*
 * A001742  Numbers whose digits contain no loops (version 2).
 * https://oeis.org/A001742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) computed by converting n to zerofree base-5, then mapping
 *            each digit d in {1..5} to {1,2,3,5,7} and interpreting as
 *            base-10
 * Range:     u8 n <= 74, u16 n <= 3280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t map[] = {0, 1, 2, 3, 5, 7};
    
    seq_t result = 0;
    seq_t power = 1;
    
    idx_t temp = n;
    while (temp > 0) {
        int digit = ((temp - 1) % 5) + 1;
        result = result + map[digit] * power;
        power = power * 10;
        temp = (temp - 1) / 5;
    }
    
    return result;
}
