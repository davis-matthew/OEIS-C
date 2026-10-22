/*
 * A000469  1 together with products of 2 or more distinct primes.
 * https://oeis.org/A000469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 1 together with products of 2+ distinct primes */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    if (n < 9) return true;
    if (n % 3 == 0) return false;
    
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    
    idx_t count = 1;
    uint64_t candidate = 2;
    
    while (1) {
        if (is_squarefree(candidate) && !is_prime(candidate)) {
            count++;
            if (count == n) return (seq_t)candidate;
        }
        candidate++;
    }
}
