/*
 * A108215  4-almost primes equal to the product of two successive semiprimes.
 * https://oeis.org/A108215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = P[n] * P[n+1] (P[k] is the k‑th semiprime, 1‑indexed)
 * Range:     u8 n <= 5, u16 n <= 81, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_array_cap_seq, ensure_array_cap_uint64, isqrt_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A108215: 4‑almost primes equal to the product of
   two successive semiprimes.  The function is memoised so that calls in
   arbitrary order are efficient. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isqrt_uint64.h"   /* helper: uint64_t isqrt(uint64_t) */
#include "../utils/ensure_array_cap_seq.h"
#include "../utils/ensure_array_cap_uint64.h"

/* ----------  Helpers for dynamic arrays  ---------- */

/* ----------  Prime storage for factorisation  ---------- */

static uint64_t *primes      = NULL;   /* array of primes */
static size_t    primes_cap  = 0;
static size_t    primes_cnt  = 0;
static uint64_t  last_prime_checked = 1;   /* largest number checked for primality */

static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= last_prime_checked) return;

    for (uint64_t n = last_prime_checked + 1; n <= limit; ++n) {
        bool is_p = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > n) break;
            if (n % p == 0) { is_p = false; break; }
        }
        if (is_p) {
            ensure_array_cap_uint64(&primes, &primes_cap, primes_cnt + 1);
            primes[primes_cnt++] = n;
        }
    }
    last_prime_checked = limit;
}

/* ----------  Semiprime generation  ---------- */

static seq_t *semiprimes = NULL;        /* 0‑based array of semiprimes   */
static size_t  semis_cap  = 0;
static size_t  semis_cnt  = 0;
static uint64_t last_candidate = 3;     /* last integer tested for semiprimality */

/* true if x has exactly two prime factors (with multiplicity) */
static bool is_semiprime(uint64_t x)
{
    if (x < 4) return false;           /* 4 = 2*2 is the first semiprime */

    uint64_t limit = isqrt_uint64(x) + 1;
    ensure_primes_up_to(limit);

    uint64_t tmp = x;
    int count = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > tmp) break;
        while (tmp % p == 0) {
            ++count;
            tmp /= p;
            if (count > 2) return false;
        }
    }
    if (tmp > 1) ++count;          /* remaining prime factor */
    return count == 2;
}

static void generate_next_semiprime(void)
{
    while (true) {
        ++last_candidate;
        if (is_semiprime(last_candidate)) {
            ensure_array_cap_seq(&semiprimes, &semis_cap, semis_cnt + 1);
            semiprimes[semis_cnt++] = (seq_t)last_candidate;
            break;
        }
    }
}

static void ensure_semiprimes_up_to(size_t n) /* 0‑based index n is required */
{
    while (semis_cnt <= n) generate_next_semiprime();
}

/* ----------  Public function  ---------- */

seq_t a(idx_t n)
{
    /* Function defined for n >= 1.  Undefined behaviour for n < 1 is ignored. */
    if (n < 1) return 0;                /* guard, not used in tests */

    /* Need semiprimes at indices n-1 and n (0‑based). */
    size_t need = (size_t)n;            /* the second of the two semiprimes */
    ensure_semiprimes_up_to(need);

    seq_t sp_prev = semiprimes[n - 1];
    seq_t sp_next = semiprimes[n];

    unsigned __int128 prod = (unsigned __int128)sp_prev * (unsigned __int128)sp_next;
    return (seq_t)prod;
}
