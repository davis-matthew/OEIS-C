/*
 * A187073  Composite squarefree numbers whose average prime factor is a prime
 *          number.
 * https://oeis.org/A187073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 2604, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Composite squarefree numbers whose average prime factor is a prime
   Computation: brute‑force search from 4 upwards, accumulating qualifying
   numbers until the requested index is reached.  Primes are generated on
   demand using a simple incremental sieve. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/_floor_isqrt_u64.h"

/* ---------- prime generator up to a given limit ---------- */
static uint64_t *prime_list = NULL;    /* sorted list of primes */
static size_t prime_count = 0;         /* number of primes stored     */
static size_t prime_cap   = 0;         /* size of the allocated array */
static uint64_t prime_limit = 1;       /* highest number tested so far*/

static void extend_primes(uint64_t limit)
{
    if (limit <= prime_limit)
        return;

    for (uint64_t n = prime_limit + 1; n <= limit; ++n) {
        bool is_p = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > n)
                break;
            if (n % p == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p) {
            if (prime_count == prime_cap) {
                prime_cap = prime_cap ? prime_cap << 1 : 64;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_count++] = n;
        }
    }
    prime_limit = limit;
}

/* ---------- helper: check candidate ---------- */
static bool candidate_ok(uint64_t x)
{
    /* skip primes and powers of primes */
    if (x <= 3)
        return false;

    /* factor x, ensuring squarefree and composite */
    uint64_t tmp = x;
    uint64_t sum = 0;
    size_t  pf_cnt = 0;
    bool squarefree = true;

    uint64_t root = _floor_isqrt_u64(tmp);
    extend_primes(root);

    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            ++pf_cnt;
            sum += p;
            tmp /= p;
            if (tmp % p == 0) {
                squarefree = false;   /* factor repeated */
                break;
            }
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (!squarefree)
        return false;
    if (tmp > 1) {  /* remaining prime factor */
        ++pf_cnt;
        sum += tmp;
    }
    if (pf_cnt < 2)          /* must be composite */
        return false;

    /* average of prime factors */
    if (sum % pf_cnt != 0)
        return false;
    uint64_t avg = sum / pf_cnt;

    /* average must be prime */
    return is_prime_big((seq_t)avg);
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined */
    uint64_t count = 0;
    uint64_t candidate = 4;           /* first composite sqrt‑free number */

    while (count < n) {
        if (candidate_ok(candidate))
            ++count;
        if (count == n)
            return (seq_t)candidate;
        ++candidate;
    }

    /* unreachable */
    return 0;
}
