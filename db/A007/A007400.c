/*
 * A007400  Continued fraction for Sum_{n>=0} 1/2^(2^n) =
 *          0.8164215090218931...
 * https://oeis.org/A007400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=4; for n>2: a(8k)=a(8k+3)=2;
 *            a(8k+4)=a(8k+7)=a(16k+5)=a(16k+14)=4; a(16k+6)=a(16k+13)=6;
 *            a(8k+1)=a(4k+1); a(8k+2)=a(4k+2)
 * Range:     u8 n <= 9194, u16 n <= 9194, u32 n <= 9194, u64 n <= 9194,
 *            u128 n <= 9194, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 4;
    
    idx_t r8 = n % 8;
    
    if (r8 == 0 || r8 == 3) {
        return 2;
    } else if (r8 == 1) {
        return a((n + 1) / 2);
    } else if (r8 == 2) {
        return a((n + 2) / 2);
    } else if (r8 == 4 || r8 == 7) {
        return 4;
    } else if (r8 == 5) {
        return (n % 16 == 5) ? 4 : 6;
    } else { // r8 == 6
        return (n % 16 == 6) ? 6 : 4;
    }
}
