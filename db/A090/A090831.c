/*
 * A090831  a(n) = prime(n)^1 + prime(n-1)^2 + prime(n-2)^3 + ... +
 *          prime(1)^n.
 * https://oeis.org/A090831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n)^1 + prime(n-1)^2 + … + prime(1)^n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_pow.h"

/* internal prime array, 1‑based sequence is stored 0‑indexed */
static int *prime_list = NULL;
static idx_t prime_count = 0;
static idx_t prime_capacity = 0;

/* add a prime to the list, expanding storage as needed */
static void add_prime(int p) {
    if (prime_count == prime_capacity) {
        prime_capacity = (prime_capacity == 0) ? 16 : prime_capacity * 2;
        prime_list = (int *)realloc(prime_list, prime_capacity * sizeof(int));
    }
    prime_list[prime_count++] = p;
}

/* generate at least n primes (so prime(n) exists) */
static void generate_primes_upto(idx_t n) {
    if (prime_count >= n) return;
    int candidate = (prime_count == 0) ? 2 : prime_list[prime_count - 1] + 1;
    while (prime_count < n) {
        int is_prime = 1;
        for (idx_t i = 0; i < prime_count; ++i) {
            int p = prime_list[i];
            if ((int64_t)p * p > candidate) break;
            if (candidate % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) add_prime(candidate);
        ++candidate;
    }
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* defined only for n≥1 */
    generate_primes_upto(n);
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t p = (seq_t)prime_list[i - 1];   /* prime(i) */
        idx_t exp = n - i + 1;
        sum += seq_pow(p, exp);
    }
    return sum;
}
