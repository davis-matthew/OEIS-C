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
 * Validated: standard, pass, 66 terms through n = 9961 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt
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
#include "../utils/isqrt.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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

    uint64_t root = isqrt(tmp);
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
