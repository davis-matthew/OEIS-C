/*
 * A006084  Continued fraction for e/3.
 * https://oeis.org/A006084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction for e/3. First nine terms: a(0)=0, a(1)=1,
 *            a(2)=9, a(3)=1, a(4)=1, a(5)=1, a(6)=5, a(7)=1, a(8)=1 For n >=
 *            9, write n = 9k + r where 0 <= r < 9: a(9k) = 1, a(9k+1) = 1,
 *            a(9k+2) = 18k+8, a(9k+3) = 1, a(9k+4) = 1, a(9k+5) = 2k+1,
 *            a(9k+6) = 5, a(9k+7) = 1, a(9k+8) = 2k+1
 * Range:     u8 n <= 127, u16 n <= 9404, u32 n <= 9404, u64 n <= 9404,
 *            u128 n <= 9404, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 9) {
        static seq_t first_nine[] = {0, 1, 9, 1, 1, 1, 5, 1, 1};
        return first_nine[n];
    }
    
    seq_t k = (seq_t)(n / 9);
    idx_t r = n % 9;
    
    switch (r) {
        case 0: return 1;
        case 1: return 1;
        case 2: return 18 * k + 8;
        case 3: return 1;
        case 4: return 1;
        case 5: return 2 * k + 1;
        case 6: return 5;
        case 7: return 1;
        case 8: return 2 * k + 1;
    }
    
    return 0;
}
