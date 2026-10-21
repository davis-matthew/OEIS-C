/*
 * A088256  Primorial numbers k such that both k-1 and k+1 are prime.
 * https://oeis.org/A088256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn, bref
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   is_prime_big
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
#include "../utils/is_prime_big.h"

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
                seq_t *tmp = (seq_t *)realloc(found, found_cap * sizeof(seq_t));
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
