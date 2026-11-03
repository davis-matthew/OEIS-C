/*
 * A002217  Starting with n, repeatedly calculate the sum of prime factors
 *          (with repetition) of the previous term, until reaching 0 or a
 *          fixed point: a(n) is the number of terms in the resulting
 *          sequence.
 * https://oeis.org/A002217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n, repeatedly compute the sum of prime factors (with
 *            repetition) until reaching 0 or a fixed point; return the count
 *            of terms in the resulting sequence
 * Range:     u8 n <= 2409, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9704 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t compute_sum_of_prime_factors(seq_t n) {
    if (n <= 1) return 0;
    
    seq_t sum = 0;
    seq_t temp = n;
    
    // Divide by 2
    while (temp % 2 == 0) {
        sum += 2;
        temp /= 2;
    }
    
    // Divide by odd numbers
    for (seq_t i = 3; i <= temp / i; i += 2) {
        while (temp % i == 0) {
            sum += i;
            temp /= i;
        }
    }
    
    // If temp > 1, it's a prime factor
    if (temp > 1) {
        sum += temp;
    }
    
    return sum;
}

seq_t a(idx_t n) {
    seq_t current = (seq_t)n;
    seq_t count = 1;
    
    while (1) {
        if (current == 0) {
            break;
        }
        
        seq_t sum_of_prime_factors = compute_sum_of_prime_factors(current);
        
        if (sum_of_prime_factors == current) {
            break;
        }
        
        current = sum_of_prime_factors;
        count++;
    }
    
    return count;
}
