/*
 * A007608  Nonnegative integers in base -4.
 * https://oeis.org/A007608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nice, easy, nonn
 * Formula:   base -4 representation of n
 * Range:     u8 n <= 35, u16 n <= 819, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t result = 0;
    seq_t place = 1;
    
    int64_t temp = (int64_t)n;
    while (temp != 0) {
        int64_t q = temp / (-4);
        int64_t r = temp % (-4);
        
        if (r < 0) {
            q++;
            r += 4;
        }
        
        result += (seq_t)r * place;
        place *= 10;
        temp = q;
    }
    
    return result;
}
