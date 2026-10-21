/*
 * A059642  Primes p such that x^55 = 2 has no solution mod p.
 * https://oeis.org/A059642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _floor_isqrt_u64, gcd_uint64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A059642: Primes p for which x^55 = 2 has no solution mod p.
   Implementation: generate primes sequentially and test each with
   2^((p−1)/gcd(p−1,55)) ≠ 1.  The primes satisfying the condition
   are stored in a static array and expanded on demand. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/mod_pow.h"
#include "../utils/gcd_uint64.h"

/* ---------- Prime generation for uint64_t ---------- */
static uint64_t *prime_list = NULL;
static size_t primes_cap = 0, primes_cnt = 0;

static void grow_prime_list(size_t need)
{
    if (primes_cap < need) {
        size_t new_cap = primes_cap ? (primes_cap << 1) : 8;
        while (new_cap < need) new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_list = tmp;
        primes_cap = new_cap;
    }
}

static uint64_t next_prime(void)
{
    if (primes_cnt == 0) {
        grow_prime_list(1);
        prime_list[primes_cnt++] = 2;
        return 2;
    }
    uint64_t cand = prime_list[primes_cnt - 1] + 1;
    for (;;) {
        if (cand > 2 && (cand & 1ULL) == 0) { cand++; continue; }
        bool prime = true;
        uint64_t sq = _floor_isqrt_u64(cand);
        for (size_t i = 0; i < primes_cnt && prime_list[i] <= sq; ++i) {
            if (cand % prime_list[i] == 0) { prime = false; break; }
        }
        if (prime) {
            grow_prime_list(primes_cnt + 1);
            prime_list[primes_cnt++] = cand;
            return cand;
        }
        cand++;
    }
}

/* ---------- Utility: gcd for uint64_t ---------- */

/* ---------- Storage for qualifying primes ---------- */
static uint64_t *seq_primes_list = NULL;
static size_t seq_cap = 0, seq_cnt = 0;

static void grow_seq_primes(size_t need)
{
    if (seq_cap < need) {
        size_t new_cap = seq_cap ? (seq_cap << 1) : 8;
        while (new_cap < need) new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(seq_primes_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        seq_primes_list = tmp;
        seq_cap = new_cap;
    }
}

/* ---------- Ensure we have at least n qualifying primes ---------- */
static void ensure_seq_n(idx_t n)
{
    while (seq_cnt < n) {
        uint64_t p = next_prime();
        if (p == 2) continue;                  /* 2 always satisfies the equation */
        uint64_t pm1 = p - 1;
        uint64_t g = gcd_uint64(pm1, 55ULL);
        uint64_t exp = pm1 / g;
        seq_t r = mod_pow((seq_t)2, exp, (seq_t)p);
        if (r != (seq_t)1) {                   /* no solution exists */
            grow_seq_primes(seq_cnt + 1);
            seq_primes_list[seq_cnt++] = p;
        }
    }
}

/* ---------- Required API function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* undefined for n < 1 */
    ensure_seq_n(n);
    return (seq_t)seq_primes_list[n - 1];
}
