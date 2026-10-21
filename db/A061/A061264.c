/*
 * A061264  Number of cyclic permutations of the digits of n which give
 *          primes.
 * https://oeis.org/A061264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of cyclic permutations of the digits of n that are
 *            prime numbers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the required value by
   1. turning n into a decimal digit array;
   2. generating all cyclic rotations of the digits;
   3. checking each rotation against a sieve of Eratosthenes.
   The sieve is extended lazily to the largest rotation value needed
   for any call.  All operations use only integer arithmetic and
   standard C headers, so the code is portable to C11 and C++17. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Global prime table cached between calls */
static bool *prime_table = NULL;
static size_t prime_table_size = 0;

/* Ensure the prime table contains all values up to 'limit' */
static void ensure_prime_table(size_t limit)
{
    if (prime_table && prime_table_size >= limit + 1)
        return;                     /* already sufficient */

    /* free old table, if any */
    free(prime_table);

    /* allocate new table */
    prime_table_size = limit + 1;
    prime_table = (bool *)calloc(prime_table_size, sizeof(bool));
    if (!prime_table) {             /* allocation failed */
        prime_table_size = 0;
        return;
    }

    /* Sieve of Eratosthenes */
    if (prime_table_size > 0) prime_table[0] = false;
    if (prime_table_size > 1) prime_table[1] = false;
    for (size_t i = 2; i < prime_table_size; ++i)
        prime_table[i] = true;

    for (size_t p = 2; p * p < prime_table_size; ++p) {
        if (prime_table[p]) {
            for (size_t j = p * p; j < prime_table_size; j += p)
                prime_table[j] = false;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* n < 1 case, though unused */

    /* 1. Extract decimal digits into an array (most‑significant first) */
    idx_t tmp = n;
    /* maximum number of decimal digits for 64‑bit n is 20 */
    unsigned char digits[20];
    size_t len = 0;
    do {
        digits[len++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    } while (tmp);

    /* reverse to normal order */
    for (size_t i = 0; i < len / 2; ++i) {
        unsigned char t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* 2. Compute all rotations and find the maximum rotation value */
    size_t max_rot = 0;
    for (size_t off = 0; off < len; ++off) {
        size_t value = 0;
        for (size_t i = 0; i < len; ++i) {
            value = value * 10 + digits[(i + off) % len];
        }
        if (value > max_rot) max_rot = value;
    }

    /* 3. Ensure prime table covers rotations */
    ensure_prime_table(max_rot);

    /* 4. Count how many rotations are prime */
    size_t count = 0;
    for (size_t off = 0; off < len; ++off) {
        size_t value = 0;
        for (size_t i = 0; i < len; ++i) {
            value = value * 10 + digits[(i + off) % len];
        }
        if (value < prime_table_size && prime_table[value])
            ++count;
    }

    return (seq_t)count;
}
