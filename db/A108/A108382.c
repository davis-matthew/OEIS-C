/*
 * A108382  Primes p such that p's set of distinct digits is {1,3,7}.
 * https://oeis.org/A108382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 30, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose distinct digit set is exactly {1,3,7}
   Computed by generating all numbers consisting of the digits 1,3,7,
   requiring that each digit appears at least once, testing primality
   with is_prime, and collecting the resulting primes in ascending order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Forward declarations */
static void generate_length(size_t len);
static void recurse_digits(uint64_t prefix, unsigned mask, size_t rem);
static void add_prime(seq_t val);

/* Storage for found primes */
static seq_t *prime_list = NULL;
static size_t prime_list_size = 0;
static size_t prime_list_cap  = 0;

/* The three allowed digits and their mask bits */
static const int  digits[3] = {1, 3, 7};
static const unsigned mask_bits[3] = {1u << 0, 1u << 1, 1u << 2};

/* Append a prime to the list, growing the array as needed */
static void add_prime(seq_t val) {
    if (prime_list_size == prime_list_cap) {
        size_t new_cap = (prime_list_cap == 0) ? 16 : prime_list_cap * 2;
        seq_t *tmp = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
        prime_list = tmp;
        prime_list_cap = new_cap;
    }
    prime_list[prime_list_size++] = val;
}

/* Recursive generation of numbers of a given length.
   'prefix' holds the already-built part; 'mask' indicates which of the
   digits 1,3,7 have appeared so far; 'rem' is the number of digits yet
   to add.  When all digits are placed, check that mask == 0b111 and the
   number is prime, then store it. */
static void recurse_digits(uint64_t prefix, unsigned mask, size_t rem) {
    if (rem == 0) {
        if ((mask & 7u) == 7u && is_prime(prefix))
            add_prime((seq_t)prefix);
        return;
    }
    for (int i = 0; i < 3; ++i) {
        uint64_t next = prefix * 10ULL + (uint64_t)digits[i];
        recurse_digits(next, mask | mask_bits[i], rem - 1);
    }
}

/* Generate all eligible primes with exactly 'len' digits. */
static void generate_length(size_t len) {
    recurse_digits(0ULL, 0u, len);
}

/* Return the nth term (1‑based). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n<1 */

    /* Produce primes until we have at least n of them. */
    static size_t current_len = 1;
    while (prime_list_size < (size_t)n) {
        generate_length(current_len++);
    }
    return prime_list[n - 1];
}
