/*
 * A007963  Number of (unordered) ways of writing 2n+1 as a sum of 3 odd
 *          primes.
 * https://oeis.org/A007963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 186, u16 n <= 6517, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/* Count the ways to write 2n+1 as p+q+r where p <= q <= r are odd primes (sieve and enumeration) */

seq_t a(idx_t n) {
    seq_t target = (seq_t)2 * (seq_t)n + (seq_t)1;
    long long target_ll = (long long)target;
    
    if (target_ll < 9) return 0;
    
    bool *is_prime = (bool *)malloc((size_t)(target_ll + 1) * sizeof(bool));
    if (!is_prime) return 0;
    
    memset(is_prime, true, (size_t)(target_ll + 1) * sizeof(bool));
    is_prime[0] = is_prime[1] = false;
    
    for (long long i = 2; i * i <= target_ll; i++) {
        if (is_prime[i]) {
            for (long long j = i * i; j <= target_ll; j += i) {
                is_prime[j] = false;
            }
        }
    }
    
    seq_t count = 0;
    for (long long p = 3; p <= target_ll / 3; p += 2) {
        if (!is_prime[p]) continue;
        
        for (long long q = p; q <= (target_ll - p) / 2; q += 2) {
            if (!is_prime[q]) continue;
            
            long long r = target_ll - p - q;
            if (r >= q && is_prime[r]) {
                count++;
            }
        }
    }
    
    free(is_prime);
    return count;
}
