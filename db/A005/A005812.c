/*
 * A005812  Weight of balanced ternary representation of n.
 * https://oeis.org/A005812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(3n) = a(n), a(3n+1) = a(n) + 1, a(9n+2) = a(n) + 2, a(9n+5) =
 *            a(3n+2) + 1, a(9n+8) = a(3n+2)
 * Range:     u8 n <= 9168, u16 n <= 9168, u32 n <= 9168, u64 n <= 9168,
 *            u128 n <= 9168, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    
    while (n > 0) {
        idx_t r = n % 9;
        idx_t k = n / 9;
        
        switch (r) {
            case 0:
                n = 3 * k;
                break;
            case 1:
                result += 1;
                n = 3 * k;
                break;
            case 2:
                result += 2;
                n = k;
                break;
            case 3:
                n = 3 * k + 1;
                break;
            case 4:
                result += 1;
                n = 3 * k + 1;
                break;
            case 5:
                result += 1;
                n = 3 * k + 2;
                break;
            case 6:
                n = 3 * k + 2;
                break;
            case 7:
                result += 1;
                n = 3 * k + 2;
                break;
            case 8:
                n = 3 * k + 2;
                break;
        }
    }
    
    return result;
}
