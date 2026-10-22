/*
 * A006530  Gpf(n): greatest prime dividing n, for n >= 2; a(1)=1.
 * https://oeis.org/A006530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, core
 * Formula:   greatest prime dividing n, for n >= 2; a(1)=1
 * Range:     u8 n <= 214, u16 n <= 9878, u32 n <= 9878, u64 n <= 9878,
 *            u128 n <= 9878, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;
    
    seq_t gpf = 1;
    seq_t m = (seq_t)n;
    
    // Divide by 2 as many times as possible
    while (m % 2 == 0) {
        gpf = 2;
        m = m / 2;
    }
    
    // Try odd divisors starting from 3
    for (seq_t i = 3; i * i <= m; i += 2) {
        while (m % i == 0) {
            gpf = i;
            m = m / i;
        }
    }
    
    // If m > 1, then it's a prime factor
    if (m > 1)
        gpf = m;
    
    return gpf;
}
