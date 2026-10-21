/*
 * A072592  Even numbers with at least one prime factor of form 4*k+1.
 * https://oeis.org/A072592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) are even numbers having at least one prime factor ≡ 1 (mod
 *            4). Computed by scanning the even integers in order and
 *            performing trial division with a pre‑computed list of small
 *            primes. The sequence is memoised so subsequent calls are fast
 *            and independent of call order
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Note: seq_t and idx_t are provided by the build system. */

seq_t a(idx_t n)
{
    /* Memoisation array: memo[0] is unused (to make 1‑based indexing simple). */
    static seq_t *memo = NULL;
    static size_t memo_cap = 0;
    static size_t memo_len = 0;   /* number of elements stored (including dummy) */

    /* Prime sieve – used only once, stored in static array. */
    enum { MAX_PRIMES = 2000000, PRIMES_SIEVE_LIMIT = 2000000 };
    static int primes[MAX_PRIMES];
    static size_t primes_len = 0;
    static bool primes_ready = false;

    /* The next even integer to examine.  It starts at 2. */
    static seq_t next_candidate = 2;

    /* Initialise the memoisation array the first time this function is called. */
    if (memo_len == 0) {
        memo_cap = 16;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo) return 0;          /* out of memory: return 0 as sentinel */
        memo[0] = 0;                  /* dummy value for 1‑based indexing */
        memo_len = 1;
    }

    /* Build the list of primes once. */
    if (!primes_ready) {
        bool *is_prime = (bool *)malloc((PRIMES_SIEVE_LIMIT + 1) * sizeof(bool));
        if (!is_prime) return 0;          /* out of memory */
        for (int i = 0; i <= PRIMES_SIEVE_LIMIT; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;
        for (int i = 2; (long long)i * i <= PRIMES_SIEVE_LIMIT; ++i) {
            if (is_prime[i]) {
                for (long long j = (long long)i * i; j <= PRIMES_SIEVE_LIMIT; j += i)
                    is_prime[(size_t)j] = false;
            }
        }
        for (int i = 2; i <= PRIMES_SIEVE_LIMIT; ++i)
            if (is_prime[i])
                primes[primes_len++] = i;
        free(is_prime);
        primes_ready = true;
    }

    /* Generate terms until we have the desired n-th term. */
    while (memo_len <= n) {
        seq_t x = next_candidate;
        bool has1mod4 = false;

        /* Trial division to test for a 1 mod 4 prime factor. */
        seq_t temp = x;
        for (size_t i = 0; i < primes_len; ++i) {
            int p = primes[i];
            seq_t p2 = (seq_t)p * (seq_t)p;
            if (p2 > temp) break;        /* sqrt of remaining part */
            if (temp % p == 0) {
                if (p % 4 == 1) { has1mod4 = true; break; }
                while (temp % p == 0) temp /= p;
            }
        }
        if (!has1mod4 && temp > 1) {
            if (temp % 4 == 1) has1mod4 = true;
        }

        if (has1mod4) {
            if (memo_len == memo_cap) {
                memo_cap <<= 1;
                memo = (seq_t *)realloc(memo, memo_cap * sizeof(seq_t));
                if (!memo) return 0;      /* out of memory */
            }
            memo[memo_len++] = x;
        }

        next_candidate += 2; /* only even numbers need to be checked */
    }

    return memo[n];
}
