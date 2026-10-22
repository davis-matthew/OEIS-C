/*
 * A004087  Primes written backwards.
 * https://oeis.org/A004087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A004086(A000040(n)), where A000040(n) is the n-th prime
 *            and A004086 reverses its digits
 * Range:     u8 n <= 26, u16 n <= 1229, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool is_prime(uint64_t m)
{
    if (m < 2) return false;
    if (m == 2) return true;
    if (m % 2 == 0) return false;
    
    for (uint64_t i = 3; i * i <= m; i += 2) {
        if (m % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t candidate = 2;
    
    // Find the n-th prime
    while (1) {
        if (is_prime(candidate)) {
            count++;
            if (count == n) {
                // Reverse the digits of candidate
                uint64_t prime = candidate;
                uint64_t reversed = 0;
                while (prime > 0) {
                    reversed = reversed * 10 + (prime % 10);
                    prime /= 10;
                }
                return (seq_t)reversed;
            }
        }
        
        candidate = (candidate == 2) ? 3 : candidate + 2;
    }
}
