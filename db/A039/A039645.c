/*
 * A039645  Number of steps to fixed point of "k -> k/2 or (k+1)/2 until
 *          result is prime", starting with prime(n)+1.
 * https://oeis.org/A039645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 118 terms through n = 9726 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A039645: Number of steps to reach a prime by repeatedly halving or
   (k+1)/2, starting from prime(n)+1 (n ≥ 1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* sieve \(1\dots limit\), returning the primes and their count */
static void sieve_to_limit(idx_t limit, uint64_t **out, size_t *cnt)
{
    bool *composite = (bool*)calloc((size_t)limit + 1, sizeof(bool));
    if (!composite) { *out = NULL; *cnt = 0; return; }

    size_t estimated = (size_t)(limit / 2) + 10;
    uint64_t *tmp_primes = (uint64_t*)malloc(estimated * sizeof(uint64_t));
    if (!tmp_primes) { free(composite); *out = NULL; *cnt = 0; return; }
    size_t pc = 0;

    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (!composite[i]) {
            if (pc == estimated) {
                estimated = estimated * 3 / 2 + 10;
                uint64_t *newbuf = (uint64_t*)realloc(tmp_primes, estimated * sizeof(uint64_t));
                if (!newbuf) { free(tmp_primes); free(composite); *out = NULL; *cnt = 0; return; }
                tmp_primes = newbuf;
            }
            tmp_primes[pc++] = (uint64_t)i;

            if (i * i <= (size_t)limit) {
                for (uint64_t j = i * i; j <= (size_t)limit; j += i)
                    composite[j] = true;
            }
        }
    }

    free(composite);

    uint64_t *pr = (uint64_t*)realloc(tmp_primes, pc * sizeof(uint64_t));
    if (!pr) { free(tmp_primes); *out = NULL; *cnt = 0; return; }

    *out = pr;
    *cnt = pc;
}

/* static cache of primes */
static uint64_t *prime_tbl = NULL;
static size_t   prime_cnt = 0;

/* ensure that we have at least n primes in prime_tbl */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n) return;

    idx_t limit = (idx_t)(n * 20) + 100;   /* good heuristic */
    while (true) {
        uint64_t *new_tbl = NULL;
        size_t    new_cnt = 0;
        sieve_to_limit(limit, &new_tbl, &new_cnt);
        if (new_tbl) {
            free(prime_tbl);
            prime_tbl = new_tbl;
            prime_cnt = new_cnt;
        }
        if (prime_cnt >= (size_t)n) break;
        limit *= 2;
    }
}

/* primality test using the cached prime table */
static bool is_prime_cached(uint64_t v)
{
    if (v < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_tbl[i];
        if (p * p > v) break;
        if (v % p == 0) return false;
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* sequence starts at 1 */

    ensure_primes(n);                /* need at least n primes */

    uint64_t current = prime_tbl[n - 1] + 1;
    seq_t    steps = 1;               /* count the starting value */

    while (!is_prime_cached(current)) {
        if (current & 1ULL)          /* odd */
            current = (current + 1ULL) >> 1;
        else                         /* even */
            current >>= 1;
        steps = steps + 1;
    }

    return steps;
}
