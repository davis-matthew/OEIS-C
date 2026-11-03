/*
 * A127305  Primes p such that p + (sum of the prime factors of p-1) + (sum of
 *          prime factors of p+1) is prime.
 * https://oeis.org/A127305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p such that p + (sum of prime factors of p-1) + (sum of
 *            prime factors of p+1) is a prime. a(1)=2, a(2)=13, a(3)=47, …
 * Range:     u8 n <= 3, u16 n <= 48, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 956 (2026-11-03)
 * Helpers:   build_primes_upto, ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises found terms and builds them on demand
   using a simple prime sieve and trial division.  It works for n up
   to about 10 000 comfortably in the 2‑second limit. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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

/* ----------  Memoised state  ---------- */
static seq_t *seq_vals   = NULL;   /* cached terms                  */
static size_t seq_cnt    = 0;       /* number of terms stored so far */
static size_t seq_cap    = 0;       /* current array capacity        */

/* Prime list used for factorisation of numbers up to a limit. */
static uint64_t *prime_list = NULL;   /* pre‑computed primes            */
static size_t prime_cnt   = 0;         /* count of primes in prime_list  */
static uint64_t prime_lim = 0;         /* largest n for which primes are pre‑computed */

/* Next candidate prime to test.  Uses uint64_t for simplicity. */
static uint64_t next_candidate = 2;

/* ----------  Helper: expand prime list to cover limit  ---------- */
static void ensure_prime_list(uint64_t limit)
{
    if (limit <= prime_lim) return;

    uint64_t *new_list = NULL;
    size_t new_cnt    = 0;
    build_primes_upto(limit, &new_list, &new_cnt);

    if (prime_list) free(prime_list);
    prime_list = new_list;
    prime_cnt  = new_cnt;
    prime_lim  = limit;
}

/* ----------  Helper: sum of prime factors with multiplicity  ---------- */
static seq_t sum_prime_factors(uint64_t n)
{
    if (n <= 1) return 0;

    /* Need primes up to sqrt(n) */
    uint64_t sf = (uint64_t)floor_sqrt((seq_t)n);
    ensure_prime_list(sf);

    seq_t sum = 0;
    uint64_t tmp = n;
    size_t i = 0;

    while (tmp > 1 && i < prime_cnt) {
        uint64_t p = prime_list[i];
        if (p * p > tmp) break;                /* remaining factor is prime   */
        while (tmp % p == 0) {
            sum += (seq_t)p;
            tmp /= p;
        }
        ++i;
    }
    if (tmp > 1)                /* last remaining prime factor */
        sum += (seq_t)tmp;

    return sum;
}

/* ----------  Sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; undefined for 0 */
        return 0;

    /* Grow cache until it contains at least n terms. */
    while (seq_cnt < (size_t)n) {
        /* --- search for the next qualifying prime --- */
        for (;;) {
            /* Skip even numbers after 2. */
            if (next_candidate > 2 && (next_candidate & 1) == 0)
                next_candidate++;

            /* Test primality of the candidate. */
            if (is_prime_big((seq_t)next_candidate)) {
                uint64_t p  = next_candidate;
                uint64_t m1 = p - 1;   /* p‑1  */
                uint64_t m2 = p + 1;   /* p+1  */

                seq_t sum1 = sum_prime_factors(m1);
                seq_t sum2 = sum_prime_factors(m2);

                seq_t total = (seq_t)p + sum1 + sum2;
                if (is_prime_big(total)) {
                    /* Store the prime in the cache. */
                    if (seq_cnt == seq_cap) {
                        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
                        seq_t *tmp = mem_resize(seq_t, seq_vals, new_cap);
                        if (!tmp) abort();
                        seq_vals = tmp;
                        seq_cap  = new_cap;
                    }
                    seq_vals[seq_cnt++] = (seq_t)p;
                    /* Advance to the next candidate for future searches. */
                    next_candidate += 2;
                    break;            /* term added; exit inner loop */
                }
            }

            /* Advance to the next odd number. */
            next_candidate += 2;
        }
    }

    return seq_vals[n - 1];
}
