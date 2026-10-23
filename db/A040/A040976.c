/*
 * A040976  a(n) = prime(n) - 2.
 * https://oeis.org/A040976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = prime(n) - 2
 * Range:     u8 n <= 55, u16 n <= 6543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper: integer square root, floor of sqrt(n) for n up to 2^64-1 */
static uint64_t isqrt_u64(uint64_t n) {
    uint64_t low = 0, high = n;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Static array of discovered primes */
static uint64_t *prime_array = NULL;
static idx_t prime_count = 0;
static idx_t prime_capacity = 0;

/* Ensure that the array contains at least `n` primes (prime(n)) */
static void ensure_primes(idx_t n) {
    while (prime_count < n) {
        uint64_t candidate = (prime_count == 0) ? 2ULL : prime_array[prime_count - 1] + 1ULL;
        /* Find the next prime starting from candidate */
        for (;;) {
            uint64_t sqrt_c = isqrt_u64(candidate);
            int is_prime_flag = 1;
            for (idx_t i = 0; i < prime_count && prime_array[i] <= sqrt_c; ++i) {
                if (candidate % prime_array[i] == 0) {
                    is_prime_flag = 0;
                    break;
                }
            }
            if (is_prime_flag)
                break;
            ++candidate;
        }
        /* Resize array if needed */
        if (prime_count == prime_capacity) {
            size_t new_cap = prime_capacity ? prime_capacity * 2 : 16;
            uint64_t *new_arr = (uint64_t *)realloc(prime_array, new_cap * sizeof(uint64_t));
            if (!new_arr) /* allocation failed, abort */ exit(EXIT_FAILURE);
            prime_array = new_arr;
            prime_capacity = new_cap;
        }
        prime_array[prime_count++] = candidate;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined, return 0 for safety */
    ensure_primes(n);
    seq_t result = (seq_t)prime_array[n - 1] - (seq_t)2;
    return result;
}
