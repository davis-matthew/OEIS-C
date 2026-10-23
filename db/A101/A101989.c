/*
 * A101989  Largest prime which can be formed from digits of n-th prime p(n).
 * https://oeis.org/A101989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   estimate_prime_upper_bound, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the largest prime that can be formed from
 * the decimal digits of the n‑th prime p(n).  The value is obtained
 * by backtracking through all distinct permutations of the digits
 * in descending order and testing each for primality. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/estimate_prime_upper_bound.h"

/* static storage for the list of primes */
static idx_t *prime_list = NULL;   /* array of primes, 0‑based */
static size_t prime_count = 0;     /* number of primes stored */

/* ensure that prime_list contains at least n_primes elements,
 * corresponding to the first n_primes primes. */
static void
ensure_primes(size_t n_primes)
{
    if (prime_count >= n_primes) return;          /* already available */

    /* estimate an upper bound on the n_primes-th prime */
    idx_t upper = estimate_prime_upper_bound((idx_t)n_primes);

    /* simple (non‑segmented) sieve up to upper */
    bool *sieve = (bool *)calloc((size_t)(upper + 1), sizeof *sieve);
    if (!sieve) exit(EXIT_FAILURE); /* memory error */

    for (idx_t i = 2; i * i <= upper; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= upper; j += i)
                sieve[j] = true;
        }
    }

    /* count primes up to the required index */
    size_t cnt = 0;
    for (idx_t i = 2; i <= upper && cnt < n_primes; ++i)
        if (!sieve[i]) ++cnt;

    /* allocate or reallocate the prime list */
    if (!prime_list) {
        prime_list = (idx_t *)malloc(cnt * sizeof *prime_list);
        if (!prime_list) exit(EXIT_FAILURE);
    } else {
        prime_list = (idx_t *)realloc(prime_list, cnt * sizeof *prime_list);
        if (!prime_list) exit(EXIT_FAILURE);
    }

    /* fill the array with primes */
    size_t k = 0;
    for (idx_t i = 2; i <= upper && k < cnt; ++i)
        if (!sieve[i]) prime_list[k++] = i;

    free(sieve);
    prime_count = k;
}

/* backtracking helper that builds the largest prime permutation.
 * freq[d] holds the remaining count of digit d.
 * rem   : number of digits left to place
 * cur   : current number being built
 * result: pointer to store the found prime */
static bool
dfs(int freq[10], int rem, idx_t cur, idx_t *result)
{
    if (rem == 0) {
        if (is_prime(cur)) {
            *result = cur;
            return true;
        }
        return false;
    }

    for (int d = 9; d >= 0; --d) {
        if (freq[d] == 0) continue;
        freq[d]--;
        if (dfs(freq, rem - 1, cur * 10 + (idx_t)d, result))
            return true;
        freq[d]++;                /* restore */
    }
    return false;
}

/* find largest prime permutation of the digits of prime p */
static seq_t
largest_prime_from_digits(idx_t p)
{
    int freq[10] = {0};
    int digits = 0;

    if (p == 0) {
        freq[0] = 1;
        digits = 1;
    } else {
        idx_t x = p;
        while (x > 0) {
            freq[x % 10]++;
            x /= 10;
            digits++;
        }
    }

    idx_t result = 0;
    (void)dfs(freq, digits, 0, &result);
    return (seq_t)result;
}

/* ------------------------------------------------------------- */
/*   Public sequence function                                     */
/* ------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1, but safe */

    ensure_primes((size_t)n);

    idx_t p = prime_list[n - 1];   /* primes array is 0‑based */
    return largest_prime_from_digits(p);
}
