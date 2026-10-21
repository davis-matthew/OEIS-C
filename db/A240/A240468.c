/*
 * A240468  Sum of the distinct prime divisors of the palindromes having an
 *          even number of digits.
 * https://oeis.org/A240468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 355, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A240468: a(n) = sum of the distinct prime divisors of the nth
   even‑digit decimal palindrome (palindromes are taken in increasing order). */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/build_primes_upto.h"    /* function build_primes_upto() */

/* Helper to reverse the decimal digits of a seq_t value. */
static seq_t reverse_digits(seq_t n)
{
    seq_t rev = 0;
    while (n > 0) {
        rev = rev * 10 + (n % 10);
        n   /= 10;
    }
    return rev;
}

/* Return the nth palindrome with an even number of decimal digits. */
static seq_t nth_even_digit_palindrome(idx_t n)
{
    /* Find the block of palindromes with the same half‑length. */
    idx_t m = 1;          /* half‑length of the palindrome */
    idx_t cumulative = 0; /* number of palindromes before this block */

    while (1) {
        /* Count of palindromes in this block: 9 * 10^(m-1). */
        idx_t count = 9;
        idx_t pow10 = 1;
        for (idx_t i = 1; i < m; ++i) pow10 *= 10;   /* 10^(m-1) */
        count *= pow10;

        if (n <= cumulative + count) break;
        cumulative += count;
        ++m;
    }

    idx_t offset = n - cumulative - 1; /* zero‑based index inside the block */

    /* 10^(m-1) and 10^m as seq_t values. */
    seq_t pow10m_1 = 1;
    for (idx_t i = 1; i < m; ++i) pow10m_1 *= 10;
    seq_t pow10m = pow10m_1 * 10;

    /* First half of the palindrome (no leading zeros). */
    seq_t first_half = pow10m_1 + (seq_t)offset;

    /* Construct the full palindrome. */
    seq_t rev = reverse_digits(first_half);
    return first_half * pow10m + rev;
}

/* Sum of the distinct prime divisors of a positive seq_t number. */
static seq_t sum_distinct_prime_factors(seq_t n)
{
    static uint64_t *primes = NULL;      /* primes up to 100000 */
    static size_t   primes_cnt = 0;
    static bool      ready = false;

    if (!ready) {
        build_primes_upto(100000, &primes, &primes_cnt);
        ready = true;
    }

    seq_t sum  = 0;
    seq_t tmp  = n;

    for (size_t i = 0; i < primes_cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            sum += p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) sum += tmp;   /* remaining prime factor > sqrt(n) */
    return sum;
}

/* Public entry point for the OEIS sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for index 0 */
    seq_t pal = nth_even_digit_palindrome(n);
    return sum_distinct_prime_factors(pal);
}
