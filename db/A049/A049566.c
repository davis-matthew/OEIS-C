/*
 * A049566  Primes p such that x^34 = 2 has a solution mod p.
 * https://oeis.org/A049566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrime_uint64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A049566: primes p for which there exists an integer x such that
 * x^34 ≡ 2 (mod p).  For an odd prime p this holds iff 
 * 2^((p‑1)/gcd(34,p‑1)) ≡ 1 (mod p).  The prime 2 trivially satisfies the
 * condition.  The function iterates over primes, tests the condition, and
 * memoizes the found terms so subsequent calls are fast.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_pow.h"
#include "../utils/isPrime_uint64.h"

/* Resize a dynamic array of seq_t */
static void ensure_seq_cap(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t newcap = (*cap) ? (*cap) : 8;
    while (newcap < need) newcap <<= 1;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

/* Resize a dynamic array of uint64_t */
static void ensure_uint64_cap(uint64_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t newcap = (*cap) ? (*cap) : 8;
    while (newcap < need) newcap <<= 1;
    *arr = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
    *cap = newcap;
}

/* 64‑bit GCD (Euclid) */
static uint64_t ugcd(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static state retained across calls */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0, primes_cap = 0;
    static seq_t *found   = NULL;
    static size_t found_cnt = 0, found_cap = 0;
    static uint64_t candidate = 2;
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
        /* Nothing else to do; arrays are NULL and counts 0 */
    }

    /* Generate further terms until we have at least n of them */
    while (found_cnt < n) {
        /* Test primality of candidate */
        bool prime = false;
        if (candidate == 2) {
            prime = true;
        } else if (candidate & 1ULL) {
            prime = isPrime_uint64(candidate, primes, primes_cnt);
        }

        if (prime) {
            /* Append candidate to primes list */
            ensure_uint64_cap(&primes, &primes_cap, primes_cnt + 1);
            primes[primes_cnt++] = candidate;

            /* Check the 34‑th power residue condition */
            bool good = false;
            if (candidate == 2) {
                good = true;
            } else {
                uint64_t d = ugcd(34ULL, candidate - 1);
                uint64_t exp = (candidate - 1) / d;
                seq_t res = mod_pow((seq_t)2, exp, (seq_t)candidate);
                good = (res == (seq_t)1);
            }

            if (good) {
                ensure_seq_cap(&found, &found_cap, found_cnt + 1);
                found[found_cnt++] = (seq_t)candidate;
            }
        }

        /* Advance to next candidate: 3,5,7,... */
        candidate = (candidate == 2) ? 3 : candidate + 2;
    }

    return found[n - 1];
}
