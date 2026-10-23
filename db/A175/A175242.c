/*
 * A175242  a(n) = the number of divisors of n that are palindromes when
 *          written in binary.
 * https://oeis.org/A175242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_bin_palindrome_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of divisors of n that are binary palindromes. */

#include <stdbool.h>
#include <stdint.h>

#include "../utils/factor_u64.h"
#include "../utils/is_bin_palindrome_uint64.h"

/* ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
/* Recursively enumerate divisors and count those that are binary palindromes. */
static void count_pal_divisors(uint64_t cur,
                               size_t idx,
                               const uint64_t *primes,
                               const int *exps,
                               size_t len,
                               uint64_t *cnt)
{
    if (idx == len) {
        if (is_bin_palindrome_uint64(cur))
            ++(*cnt);
        return;
    }

    uint64_t mult = 1;
    for (int e = 0; e <= exps[idx]; ++e) {
        count_pal_divisors(cur * mult, idx + 1, primes, exps, len, cnt);
        mult *= primes[idx];
    }
}

/* ---------------------------------------------------------------------- */
/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Factorise n. */
    /* For n up to 2^64, we need at most 64 distinct prime factors. */
    uint64_t pf_primes[64];
    uint64_t pf_exps[64];
    size_t pf_len = 0;

    factor_u64((uint64_t)n, pf_primes, pf_exps, &pf_len);

    /* Convert exponents to signed int for the recursive routine. */
    int exps_int[64];
    for (size_t i = 0; i < pf_len; ++i)
        exps_int[i] = (int)pf_exps[i];

    /* Count palindromic divisors. */
    uint64_t count = 0;
    count_pal_divisors(1ULL, 0, pf_primes, exps_int, pf_len, &count);

    return (seq_t)count;
}
