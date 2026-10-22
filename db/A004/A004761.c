/*
 * A004761  Numbers n whose binary expansion does not begin with 11.
 * https://oeis.org/A004761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) recurrence with a(1)=0, a(2)=1: For n > 2: a(n) = a(n-1)+1
 *            if n is odd, a(n) = 2*a(n/2+1) if n is even
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2) return 1;
    
    if (n & 1) {  // n is odd
        return a(n - 1) + 1;
    } else {  // n is even
        return 2 * a(n / 2 + 1);
    }
}
