/*
 * A117460  Primes prime(i) such that their sum-of-index-digits A007953(i) and
 *          their sum-of-digits A007605(i) are consecutive primes.
 * https://oeis.org/A117460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the n-th prime p_i such that the digit sum of the index i
 *            (sum_digits(i)) is a prime, the digit sum of p_i is a prime, and
 *            the latter is the next prime after the former. (a(1)=2 is a
 *            special inclusion.)
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   digit_sum, get_prime, is_prime_small, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_TERMS 20000

/* Utility functions provided by the test harness. */
#include "../utils/get_prime.h"     /* seq_t get_prime(idx_t k); */
#include "../utils/digit_sum.h"     /* seq_t digit_sum(seq_t v); */
#include "../utils/is_prime_small.h"/* bool is_prime_small(uint64_t n); */
#include "../utils/next_prime.h"    /* uint64_t next_prime(uint64_t n); */

/* Sum of decimal digits of an idx_t value. */
static unsigned sum_digits_idx(idx_t x)
{
    unsigned s = 0;
    while (x != 0) {
        s += (unsigned)(x % 10);
        x /= 10;
    }
    return s;
}

/* Static cache for computed terms. */
static seq_t seq[MAX_TERMS];
static size_t seq_len = 0;
static idx_t next_index = 1;      /* next prime index to examine */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range, never used */

    if (seq_len >= (size_t)n)             /* already computed */
        return seq[n - 1];

    /* Generate terms until we have enough. */
    while (seq_len < (size_t)n) {
        /* Special inclusion of the first term 2 (index 1). */
        if (next_index == 1) {
            seq[seq_len++] = (seq_t)get_prime(1);   /* prime(1) = 2 */
            ++next_index;
            continue;
        }

        seq_t p = (seq_t)get_prime(next_index);

        unsigned sum_index = sum_digits_idx(next_index);
        seq_t sum_p_s = digit_sum(p);
        uint64_t sum_p = (uint64_t)sum_p_s;

        if (is_prime_small((uint64_t)sum_index) &&
            is_prime_small(sum_p) &&
            next_prime((uint64_t)sum_index) == sum_p) {

            seq[seq_len++] = p;
        }

        ++next_index;
    }

    return seq[n - 1];
}
