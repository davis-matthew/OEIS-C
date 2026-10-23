/*
 * A024620  Positions of primes among the powers of primes (A000961).
 * https://oeis.org/A024620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (# of prime powers ≤ p_n) + 1 * where p_n is the n-th
 *            prime. The 1‑term shift matches the OEIS * definition (see
 *            a(1)=2, a(2)=3, …). This implementation builds * a sieve of
 *            primes on demand and counts, for each prime ≤ p_n, all * its
 *            powers not exceeding p_n. The function works for any n ≥...
 * Range:     u8 n <= 54, u16 n <= 6764, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>

/* ----------  Dynamic prime sieve  ---------- */
static bool *comp_flag = NULL;          /* composite flags */
static uint64_t *prime_list = NULL;     /* list of primes */
static size_t prime_cnt  = 0;           /* number of primes stored */
static idx_t   sieve_limit  = 0;        /* highest value sieved */

/* Rebuilds the sieve to at least 'limit'.  The old structure is freed. */
static void
ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;          /* already enough */

    /* free previous resources */
    free(comp_flag);
    free(prime_list);

    /* allocate composite flag array */
    comp_flag = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!comp_flag) abort();

    /* simple Eratosthenes sieve */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!comp_flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                comp_flag[j] = true;
        }
    }

    /* count primes to estimate allocation size */
    size_t est = (size_t)(limit / 10 + 10);
    prime_list = (uint64_t *)malloc(est * sizeof(uint64_t));
    if (!prime_list) abort();

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!comp_flag[i]) {
            if (cnt == est) {                /* grow table if needed */
                est <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                    est * sizeof(uint64_t));
                if (!tmp) abort();
                prime_list = tmp;
            }
            prime_list[cnt++] = (uint64_t)i;
        }
    }

    prime_cnt  = cnt;
    sieve_limit = limit;
}

/* Ensures we have at least the first 'n' primes in prime_list.
 * The sieve limit is increased using the known bound p_n ≤ n (ln n + ln ln n)
 * for n ≥ 6. */
static void
ensure_primes_needed(idx_t n)
{
    if (prime_cnt >= (size_t)n) return;

    idx_t limit;
    if (n < 6) {
        limit = 15;
    } else {
        double dn = (double)n;
        limit = (idx_t)(dn * (log(dn) + log(log(dn))));
    }
    limit += 10;                 /* safety margin */

    ensure_sieve(limit);

    /* if the new sieve still didn't yield enough primes, double the limit */
    while (prime_cnt < (size_t)n) {
        limit <<= 1;
        ensure_sieve(limit);
    }
}

/* ----------  OEIS sequence function  ---------- */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;    /* undefined for n < 1, but safe value */

    /* get nth prime  (1‑based indexing) */
    ensure_primes_needed(n);
    seq_t p_n = (seq_t)prime_list[n - 1];

    /* count prime powers ≤ p_n */
    size_t count = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= (uint64_t)p_n; ++i) {
        seq_t prime = (seq_t)prime_list[i];
        seq_t val   = prime;
        while (val <= p_n) {
            ++count;
            /* avoid overflow before next multiplication */
            if (val > p_n / prime) break;
            val *= prime;
        }
    }

    return (seq_t)(count + 1);   /* the required +1 shift */
}
