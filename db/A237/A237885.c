/*
 * A237885  a(n) is the number of ways that 4n can be written as p+q (p>q)
 *          with p, q, (p-q)/2, 4n-(p-q)/2 all prime numbers.
 * https://oeis.org/A237885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of unordered prime pairs (p,q), p>q,
   with p+q = 4n and with d = (p−q)/2 prime and r = 4n−d prime. 
   The routine builds a sieve up to 4n and counts the pairs. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*------------------------------------------------------------*/
/*  Prime sieve data                                     */
/*------------------------------------------------------------*/
static idx_t  sieve_limit = 0;          /* maximum value currently sieved   */
static uint8_t *prime_flag = NULL;      /* prime_flag[i] == 1 iff i is prime */
static uint64_t *prime_list = NULL;     /* list of primes up to sieve_limit */
static size_t   prime_cnt   = 0;        /* number of primes in prime_list   */

/*------------------------------------------------------------*/
/*  Memoisation data                                      */
/*------------------------------------------------------------*/
static seq_t  *memo = NULL;      /* memoised a(n) values, indexed by 1..n */
static size_t  memo_size = 0;    /* greatest n for which memo holds      */

/*------------------------------------------------------------*/
/*  Build Eratosthenes sieve up to limit (inclusive)      */
/*------------------------------------------------------------*/
static void build_sieve(idx_t limit)
{
    /* Allocate flag array */
    uint8_t *flag = (uint8_t*)malloc((size_t)(limit + 1));
    if (!flag) abort();
    memset(flag, 1, (size_t)(limit + 1));
    if (limit >= 0) flag[0] = 0;
    if (limit >= 1) flag[1] = 0;

    /* Sieve */
    for (idx_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i) flag[j] = 0;
        }
    }

    /* Count primes and build prime list */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (flag[i]) ++cnt;

    uint64_t *plist = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!plist) abort();

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) if (flag[i]) plist[pos++] = (uint64_t)i;

    /* Replace previous data */
    free(prime_flag);
    free(prime_list);
    prime_flag = flag;
    prime_list = plist;
    prime_cnt  = cnt;
    sieve_limit = limit;
}

/*------------------------------------------------------------*/
/*  Ensure that the sieve covers all values up to 4*n.     */
/*------------------------------------------------------------*/
static void ensure_sieve(idx_t n)
{
    idx_t needed = 4 * n;
    if (needed > sieve_limit) build_sieve(needed);
}

/*------------------------------------------------------------*/
/*  Main sequence function.                                   */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* defined only for n>=1 */

    ensure_sieve(n);

    /* Allocate and compute memoised values if needed */
    size_t up = (size_t)n;
    if (up > memo_size) {
        size_t new_size = up + 1;
        seq_t *new_mem = (seq_t*)realloc(memo, new_size * sizeof(seq_t));
        if (!new_mem) abort();
        memo = new_mem;

        for (size_t i = memo_size + 1; i <= up; ++i) {
            idx_t i_val = (idx_t)i;
            seq_t count = 0;

            /* Iterate over primes d < i_val */
            for (size_t pi = 0; pi < prime_cnt; ++pi) {
                uint64_t d = prime_list[pi];
                if (d >= i_val) break;

                uint64_t q = 2 * i_val - d;   /* q must be >= 2 */
                if (q < 2) continue;

                uint64_t p = 2 * i_val + d;
                uint64_t r = 4 * i_val - d;

                if (prime_flag[q] && prime_flag[p] && prime_flag[r])
                    ++count;
            }

            memo[i] = count;
        }
        memo_size = up;
    }

    return memo[up];
}
