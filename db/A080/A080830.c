/*
 * A080830  Primes that terminate at 68,34,17 in the 3x-1 problem: Repeat, if
 *          x is even divide by 2 else multiply by 3 and subtract 1, until 17
 *          is reached.
 * https://oeis.org/A080830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 122, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A080830: Primes that eventually reach 17 under
   the 3x-1 process (even -> /2, odd -> 3x-1).  This
   implementation tests primes by iterating the process
   until 17 is encountered or a cycle is detected. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIMIT 2000000      /* sieve limit for prime generation */
#define PRIME_CAP 200000   /* capacity for prime table */
#define PATH_MAX 4096      /* maximum path length for a single trace */

/* Memoization table for numbers <= LIMIT:
   -1: unknown, 0: does not reach 17, 1: reaches 17 */
static signed char memo[LIMIT + 1];

/* Prime table */
static uint64_t primes[PRIME_CAP];
static size_t primes_count = 0;

/* Initialization flag */
static bool initialized = false;

/* Forward declaration of helper */
static bool reaches17(seq_t x);

/*----------------------------------------*/
/* Helper: test whether x reaches 17      */
static bool reaches17(seq_t x)
{
    seq_t path[PATH_MAX];
    size_t path_len = 0;
    int result = -1;  /* -1 unknown, 0 false, 1 true */

    while (true) {
        /* If we have already computed the result for this x (<= LIMIT) */
        if (x <= (seq_t)LIMIT) {
            int m = memo[(size_t)x];
            if (m != -1) {
                result = m;
                break;
            }
        }

        /* Check if x == 17 */
        if (x == (seq_t)17) {
            result = 1;
            break;
        }

        /* Detect simple cycle: check if x already appears in current path */
        for (size_t i = 0; i < path_len; ++i) {
            if (path[i] == x) {
                result = 0;
                goto store_and_return;
            }
        }

        /* Append current x to path if space allows */
        if (path_len < PATH_MAX) {
            path[path_len++] = x;
        } else {
            /* Path too long; conservatively assume no reach */
            result = 0;
            goto store_and_return;
        }

        /* Next iteration of the 3x-1 process */
        if ((x & (seq_t)1) == 0) {
            x >>= 1;          /* even: divide by 2 */
        } else {
            x = (x * (seq_t)3) - (seq_t)1;  /* odd: 3x-1 */
        }
    }

store_and_return:
    /* Cache the result for all numbers in the path that are <= LIMIT */
    for (size_t i = 0; i < path_len; ++i) {
        if (path[i] <= (seq_t)LIMIT) {
            memo[(size_t)path[i]] = (signed char)result;
        }
    }
    return result == 1;
}

/*----------------------------------------*/
/* The sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for index 0 */

    /* One-time initialization */
    if (!initialized) {
        /* Sieve of Eratosthenes up to LIMIT */
        bool *is_prime = (bool*)malloc((LIMIT + 1) * sizeof(bool));
        for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;
        for (size_t i = 2; i * i <= LIMIT; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= LIMIT; j += i) is_prime[j] = false;
            }
        }
        for (size_t i = 2; i <= LIMIT && primes_count < PRIME_CAP; ++i) {
            if (is_prime[i]) primes[primes_count++] = (uint64_t)i;
        }
        free(is_prime);

        /* Initialize memo table, mark 17 as true */
        for (size_t i = 0; i <= LIMIT; ++i) memo[i] = -1;
        memo[17] = 1;
        initialized = true;
    }

    /* Scan primes until we find the nth one that reaches 17 */
    idx_t found = 0;
    for (size_t idx = 0; idx < primes_count; ++idx) {
        uint64_t p = primes[idx];
        if (reaches17((seq_t)p)) {
            ++found;
            if (found == n) return (seq_t)p;
        }
    }

    /* If not enough primes were pre‑computed, extend search by trial
       division beyond LIMIT (unlikely for n <= 10000). */
    uint64_t current = (uint64_t)primes[primes_count - 1] + 1;
    while (true) {
        /* skip non‑odd numbers */
        if (current % 2 == 0) { ++current; continue; }

        /* trial division using known primes */
        bool is_prime_flag = true;
        for (size_t i = 0; i < primes_count; ++i) {
            uint64_t q = primes[i];
            if (q * q > current) break;
            if (current % q == 0) { is_prime_flag = false; break; }
        }

        if (is_prime_flag) {
            /* Dynamically add it to prime table if space */
            if (primes_count < PRIME_CAP) primes[primes_count++] = current;
            if (reaches17((seq_t)current)) {
                ++found;
                if (found == n) return (seq_t)current;
            }
        }
        ++current;
    }
}
