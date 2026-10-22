/*
 * A006341  Octal palindromes which are also primes.
 * https://oeis.org/A006341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_prime, octal_str_to_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/octal_str_to_decimal.h"

/* a(n) = n-th m where m is an octal palindrome and m (as octal) is prime */

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t m = 1;
    
    while (count < n) {
        // Check if all digits are 0-7
        seq_t temp = m;
        bool valid = true;
        while (temp > 0) {
            if (temp % 10 > 7) {
                valid = false;
                break;
            }
            temp /= 10;
        }
        
        if (valid) {
            // Check if it's a palindrome
            seq_t original = m;
            seq_t reversed = 0;
            temp = m;
            while (temp > 0) {
                reversed = reversed * 10 + temp % 10;
                temp /= 10;
            }
            
            if (original == reversed) {
                // Convert octal string to decimal and check if prime
                seq_t decimal_value = octal_str_to_decimal(m);
                
                if (is_prime((uint64_t)decimal_value)) {
                    count++;
                    if (count == n) return m;
                }
            }
        }
        
        m++;
    }
    
    return 0;
}
