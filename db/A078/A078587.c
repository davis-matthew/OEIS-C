/*
 * A078587  Largest prime p such that p<n and 2n-p is also prime.
 * https://oeis.org/A078587
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest prime p < n such that 2n - p is also prime.
 *            Implementation uses a simple sieve of Eratosthenes built up to
 *            2*n on demand
 * Range:     u8 n <= 259, u16 n <= 9766, u32 n <= 9766, u64 n <= 9766,
 *            u128 n <= 9766, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of the sequence function */
seq_t a(idx_t n);

/* Static state for the prime sieve */
static bool *prime_flag = NULL;
static idx_t sieve_max = 0;

/* Build or extend a prime sieve up to at least 'limit'.  The sieve is stored in
 * prime_flag[0..limit], where prime_flag[i] is true if i is prime.  The sieve
 * is rebuilt from scratch when extended.  The cost is negligible for limits
 * up to a few hundred thousand, which covers the intended use (n ≈ 10000). */
static void build_sieve(idx_t limit)
{
    /* Allocate memory for the new sieve */
    bool *new_flag = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!new_flag) {
        /* In case of allocation failure, abort the program (not expected). */
        abort();
    }

    /* Initially assume all numbers ≥2 are prime */
    for (idx_t i = 0; i <= limit; ++i)
        new_flag[i] = true;
    if (limit >= 0) new_flag[0] = false;
    if (limit >= 1) new_flag[1] = false;

    /* Eratosthenes sieve */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (new_flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                new_flag[j] = false;
        }
    }

    /* Replace the old sieve (if any) with the new one */
    free(prime_flag);
    prime_flag = new_flag;
    sieve_max = limit;
}

seq_t a(idx_t n)
{
    if (n < 4) {
        /* The sequence is undefined for n < 4; return 0 for safety. */
        return (seq_t)0;
    }

    /* Ensure the sieve covers the range up to 2*n (needed for checking
     * 2*n - p).  If the current sieve is smaller, rebuild it larger. */
    idx_t required_limit = 2 * n;
    if (required_limit > sieve_max) {
        build_sieve(required_limit);
    }

    /* Search for the largest prime p < n with 2*n - p also prime. */
    for (idx_t p = n - 1; p >= 2; --p) {
        if (prime_flag[p] && prime_flag[2 * n - p]) {
            return (seq_t)p;
        }
    }

    /* In the unlikely event that no such prime exists (should not happen
     * for n ≥ 4), return 0 as a fallback. */
    return (seq_t)0;
}
