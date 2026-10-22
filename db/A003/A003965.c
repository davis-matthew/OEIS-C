/*
 * A003965  Fully multiplicative with a(prime(k)) = Fibonacci(k+2).
 * https://oeis.org/A003965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   If n = Product p(k)^e(k) then a(n) = Product
 *            Fibonacci(k+2)^e(k), where k is the index of prime p (1st prime
 *            = 2, 2nd prime = 3, etc.)
 * Range:     u8 n <= 36, u16 n <= 82, u32 n <= 198, u64 n <= 478,
 *            u128 n <= 1102, big any n
 * Helpers:   a000248_pow, fibonacci, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include "../utils/a000248_pow.h"
#include "../utils/fibonacci.h"

static idx_t prime_index(uint64_t p) {
    if (p < 2) return 0;
    idx_t count = 0;
    for (uint64_t i = 2; i <= p; i++) {
        if (is_prime(i)) count++;
    }
    return count;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    uint64_t temp = n;
    
    for (uint64_t p = 2; p * p <= temp; p++) {
        if (is_prime(p)) {
            idx_t e = 0;
            while (temp % p == 0) {
                e++;
                temp /= p;
            }
            if (e > 0) {
                idx_t k = prime_index(p);
                seq_t fib_val = fibonacci(k + 2);
                result *= a000248_pow(fib_val, e);
            }
        }
    }
    
    if (temp > 1) {
        idx_t k = prime_index(temp);
        seq_t fib_val = fibonacci(k + 2);
        result *= fib_val;
    }
    
    return result;
}
