/*
 * A088256  Primorial numbers k such that both k-1 and k+1 are prime.
 * https://oeis.org/A088256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn, bref
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 3 terms through n = 3 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A088256: primorial numbers k such that both k-1 and k+1 are prime.
   The function generates primorials incrementally, tests primality of k±1 using
   is_prime_big, and memoises the found terms.  It works for any seq_t and
   idx_t types provided by the build. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
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

static seq_t *found      = NULL;          /* memoised terms */
static size_t found_cnt  = 0;
static size_t found_cap  = 0;

static seq_t curPrimorial = 1;            /* product of processed primes */

static uint64_t *primeList  = NULL;       /* stored prime list */
static size_t   primeCnt    = 0;
static size_t   primeCap    = 0;
static uint64_t lastPrime   = 1;          /* last generated prime */

/* extend the primes list by one prime */
static void generate_next_prime(void)
{
    uint64_t cand;

    if (lastPrime < 2) {
        /* generate 2 as the first prime */
        lastPrime = 2;
    } else {
        cand = lastPrime + 1;
        if ((cand & 1) == 0) cand++;       /* make candidate odd */

        for (;;) {
            bool prime = true;
            for (size_t i = 0; i < primeCnt; ++i) {
                uint64_t p = primeList[i];
                __uint128_t sq = (__uint128_t)p * p;
                if (sq > cand) break;
                if ((cand % p) == 0) { prime = false; break; }
            }
            if (prime) break;
            cand += 2;                      /* skip even numbers */
        }
        lastPrime = cand;
    }

    /* store prime into list, resizing if needed */
    if (primeCap == primeCnt) {
        primeCap = primeCap ? primeCap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(primeList, primeCap * sizeof(uint64_t));
        if (!tmp) abort();
        primeList = tmp;
    }
    primeList[primeCnt++] = lastPrime;
}

/* generate terms until at least n terms are found or a hard limit is reached */
static void ensure_terms(idx_t n)
{
    const idx_t MAX_CHECKS = 20000;   /* avoid infinite loops on a very large sequence */
    idx_t checks = 0;

    while (found_cnt < (size_t)n && checks < MAX_CHECKS) {
        generate_next_prime();                     /* get next prime */
        curPrimorial *= (seq_t)lastPrime;          /* update primorial */

        seq_t a_minus = curPrimorial - 1;
        seq_t a_plus  = curPrimorial + 1;

        if (is_prime_big(a_minus) && is_prime_big(a_plus)) {
            /* store term, resizing if needed */
            if (found_cap == found_cnt) {
                found_cap = found_cap ? found_cap * 2 : 8;
                seq_t *tmp = mem_resize(seq_t, found, found_cap);
                if (!tmp) abort();
                found = tmp;
            }
            found[found_cnt++] = curPrimorial;
        }
        ++checks;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_terms(n);
    return (found_cnt >= (size_t)n) ? found[n - 1] : (seq_t)0;
}
