/*
 * A052494  Number of different primes that can be formed by permuting digits
 *          of n-th prime.
 * https://oeis.org/A052494
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   sequence by counting prime permutations of each n‑th prime
 *            (offset 2). a(n) = | { p | p is prime and p can be obtained by a
 *            permutation of the digits of the (n‑1)-th prime, leading zeros
 *            forbidden } | (n ≥ 2) For n < 2 the function returns 0
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Upper bound for the 10000‑th prime (≈104729) – safe for all required n. */
#define SIEVE_LIMIT 200000

static bool  *is_prime_flags = NULL;   /* true if index is prime  */
static int   *prime_list     = NULL;   /* array of primes in ascending order */
static size_t prime_count    = 0;       /* number of primes up to SIEVE_LIMIT  */
static seq_t *cache          = NULL;   /* memoised a(n) values; cache[0] unused */

/* ----------------------------------------------------------------------- */
/* Build an Eratosthenes sieve and the list of primes up to SIEVE_LIMIT.  */
static void init_sieve(void)
{
    if (is_prime_flags != NULL)
        return;   /* already built */

    size_t limit = SIEVE_LIMIT;
    is_prime_flags = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_prime_flags)
        return;   /* out of memory – subsequent use will crash */

    for (size_t i = 2; i <= limit; ++i)
        is_prime_flags[i] = true;

    for (size_t i = 2; (i * i) <= limit; ++i) {
        if (is_prime_flags[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime_flags[j] = false;
        }
    }

    /* Count primes first to allocate enough space */
    prime_count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime_flags[i])
            ++prime_count;

    prime_list = (int *)malloc(prime_count * sizeof(int));
    if (!prime_list)
        return;   /* out of memory */

    for (size_t i = 2, j = 0; i <= limit; ++i)
        if (is_prime_flags[i])
            prime_list[j++] = (int)i;

    /* Allocate cache (index 0 unused) */
    cache = (seq_t *)calloc(prime_count + 1, sizeof(seq_t));
}

/* ----------------------------------------------------------------------- */
/* Recursive helper: counts prime permutations of a single prime number p.   */
static void permute_rec(const int *digits, int len, bool *used,
                        int depth, int cur, seq_t *cnt)
{
    if (depth == len) {
        if (cur <= (int)SIEVE_LIMIT && is_prime_flags[cur])
            (*cnt)++;
        return;
    }
    for (int i = 0; i < len; ++i) {
        if (used[i]) continue;
        if (depth == 0 && digits[i] == 0) continue;       /* no leading zero */
        if (i > 0 && digits[i] == digits[i-1] && !used[i-1])
            continue;                                      /* avoid duplicates */
        used[i] = true;
        permute_rec(digits, len, used, depth + 1, cur * 10 + digits[i], cnt);
        used[i] = false;
    }
}

/* Count distinct prime permutations of the integer p. */
static seq_t count_prime_permutations(int p)
{
    int digits[10];
    int len = 0;
    int t = p;
    while (t > 0) {
        digits[len++] = t % 10;
        t /= 10;
    }
    /* Sort digits to enable duplicate handling */
    for (int i = 0; i < len - 1; ++i)
        for (int j = i + 1; j < len; ++j)
            if (digits[i] > digits[j]) {
                int tmp = digits[i];
                digits[i] = digits[j];
                digits[j] = tmp;
            }

    bool used[10] = {false};
    seq_t count = 0;
    permute_rec(digits, len, used, 0, 0, &count);
    return count;
}

/* ----------------------------------------------------------------------- */
/* Required OEIS function.  n is 1‑based but the sequence starts at offset 2. */
seq_t a(idx_t n)
{
    if (n < 2)                     /* indices below the offset are undefined */
        return 0;

    init_sieve();
    if (n > prime_count)           /* n would reference a prime beyond our sieve */
        return 0;

    /* Cache value if already computed */
    if (cache[n] != 0)
        return cache[n];

    /* nth term uses the (n‑1)‑th prime (1‑based) → index n‑2 in zero‑based array */
    int p = prime_list[n - 2];
    seq_t val = count_prime_permutations(p);
    cache[n] = val;
    return val;
}
