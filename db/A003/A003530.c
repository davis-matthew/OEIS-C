/*
 * A003530  Divisors of 2^21 - 1.
 * https://oeis.org/A003530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   divisors of 2^21 - 1 = 7^2 * 127 * 337, sorted in ascending
 *            order
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1 || n > 12) return (seq_t)0;
    
    static seq_t divisors[12] = {0};
    static int computed = 0;
    
    if (!computed) {
        int count = 0;
        
        for (int a = 0; a <= 2; a++) {
            for (int b = 0; b <= 1; b++) {
                for (int c = 0; c <= 1; c++) {
                    seq_t d = 1;
                    for (int i = 0; i < a; i++) d *= 7;
                    for (int i = 0; i < b; i++) d *= 127;
                    for (int i = 0; i < c; i++) d *= 337;
                    divisors[count++] = d;
                }
            }
        }
        
        for (int i = 0; i < 12; i++) {
            for (int j = i + 1; j < 12; j++) {
                if (divisors[i] > divisors[j]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }
        
        computed = 1;
    }
    
    return divisors[n - 1];
}
