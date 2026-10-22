/*
 * A003527  Divisors of 2^16 - 1.
 * https://oeis.org/A003527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   divisors of 2^16 - 1 = 65535, sorted in ascending order
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t divisors[16] = {0};
    static int computed = 0;
    
    if (!computed) {
        uint64_t num = 65535;
        int count = 0;
        
        for (uint64_t i = 1; i * i <= num; i++) {
            if (num % i == 0) {
                divisors[count++] = (seq_t)i;
                if (i != num / i) {
                    divisors[count++] = (seq_t)(num / i);
                }
            }
        }
        
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (divisors[i] > divisors[j]) {
                    seq_t temp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = temp;
                }
            }
        }
        
        computed = 1;
    }
    
    if (n < 1 || n > 16) return 0;
    return divisors[n - 1];
}
