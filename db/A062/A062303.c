/*
 * A062303  Number of ways writing the n-th prime as a sum of two nonprimes.
 * https://oeis.org/A062303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS A062303 sequence.
   a(n) = number of unordered partitions of the n‑th prime into two nonprimes
   (1 and composite numbers). */

/* ==== Helper functions and static data ==== */
#include <stdint.h>
#include <stdlib.h>

/* Prime sieve data */
static unsigned char *prime_flag = NULL;   /* 1 = prime, 0 = nonprime */
static uint64_t *prime_list   = NULL;     /* array of primes (ascending) */
static size_t       prime_count = 0;       /* number of primes stored */
static uint64_t     sieve_limit = 0;       /* last limit sieved */

/* Build a sieve of Eratosthenes up to 'limit' and fill prime_flag
   and prime_list.  Previous allocations are freed. */
static void build_sieve(uint64_t limit)
{
    free(prime_flag);
    free(prime_list);

    prime_flag = (unsigned char*)malloc(limit + 1);
    if (!prime_flag) exit(1);

    /* Initially mark everything as prime. */
    for (uint64_t i = 0; i <= limit; ++i) prime_flag[i] = 1;
    prime_flag[0] = prime_flag[1] = 0;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (prime_flag[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                prime_flag[j] = 0;
        }
    }

    /* Count primes to allocate the list. */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (prime_flag[i]) ++cnt;

    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) exit(1);

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (prime_flag[i]) prime_list[idx++] = i;

    prime_count = cnt;
    sieve_limit = limit;
}

/* Ensure that we have at least 'n' primes stored.  If not, enlarge
   the sieve and rebuild. */
static void ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n) return;

    /* Rough upper bound for the nth prime (n >= 6): n*(log n + log log n).
       For simplicity, use n*15 which is safe for n up to at least 200000. */
    uint64_t estimate = n * 15;
    if (estimate < 100) estimate = 100;

    /* If our current sieve is already large enough, increase it. */
    if (estimate <= sieve_limit) estimate = sieve_limit * 2 + 10;

    /* Rebuild until we have enough primes. */
    while (prime_count < (size_t)n) {
        build_sieve(estimate);
        estimate = sieve_limit * 2 + 10;
    }
}

/* ==== Main sequence function ==== */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n>=1 in tests */
    ensure_primes(n);

    uint64_t p = prime_list[n - 1]; /* nth prime (1‑based) */
    seq_t count = 0;

    for (uint64_t a_val = 1; a_val <= p / 2; ++a_val) {
        uint64_t b_val = p - a_val;
        if (!prime_flag[a_val] && !prime_flag[b_val]) {
            ++count;
        }
    }
    return count;
}
