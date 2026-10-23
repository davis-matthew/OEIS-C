/*
 * A131260  a(n) is the least palindrome > a(n-1) such that a(1) + a(2) + ...
 *          + a(n) is a semiprime.
 * https://oeis.org/A131260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the least palindrome > a(n-1) such that S(n) =
 *            a(1)+...+a(n) is a semiprime (product of two primes). This
 *            function memoizes terms in static arrays so that calls do not
 *            need to be monotonic. Uses helpers: - is_palindrome(seq_t,
 *            idx_t) (../utils/is_palindrome.h) -
 *            count_prime_factors(seq_t)...
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   count_prime_factors, grow_seq, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_palindrome.h"
#include "../utils/count_prime_factors.h"
#include "../utils/grow_seq.h"
#include <stdbool.h>

/* Find the smallest palindrome strictly greater than n */
static seq_t next_palindrome(seq_t n)
{
    seq_t x = n + 1;
    while (!is_palindrome(x, 10))
        ++x;
    return x;
}

/* Return true iff n is a semiprime (product of exactly two primes) */
static bool is_semiprime(seq_t n)
{
    seq_t f = count_prime_factors(n);
    return f == 2;
}

seq_t a(idx_t n)
{
    /* Static storage for memoization */
    static seq_t *terms  = NULL;
    static seq_t *sums   = NULL;
    static size_t terms_cap = 0;
    static size_t sums_cap  = 0;
    static size_t terms_size = 0;

    if (n == 0) return 0; /* undefined, but guard */

    /* Ensure capacity for up to n terms */
    if (terms_cap < (size_t)n)
        grow_seq(&terms, &terms_cap, (size_t)n);
    if (sums_cap  < (size_t)n)
        grow_seq(&sums,  &sums_cap,  (size_t)n);

    /* Compute missing terms up to n */
    for (size_t i = terms_size; i < (size_t)n; ++i) {
        seq_t cand = next_palindrome((i == 0) ? 0 : terms[i-1]);

        seq_t sum_candidate;
        if (i == 0)
            sum_candidate = cand;
        else
            sum_candidate = sums[i-1] + cand;

        while (!is_semiprime(sum_candidate)) {
            cand = next_palindrome(cand);
            sum_candidate = (i == 0) ? cand : sums[i-1] + cand;
        }

        terms[i] = cand;
        sums[i]  = sum_candidate;
    }

    terms_size = (size_t)n;
    return terms[n-1];
}
