/*
 * A052053  Digits of the prime factors of composite a(n) are not shared.
 * https://oeis.org/A052053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A052053 – the nth composite number whose distinct prime factors
 * have pairwise disjoint decimal digit sets.  The code searches positive
 * integers, sieves primes up to 10 000 000, factors each candidate with
 * trial division, checks digit‐mask conflicts and counts valid numbers
 * until the desired index is reached. */

/* --- utility includes --------------------------------------------------- */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

/* --- static prime list ----------------------------------------------- */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;

/* Build a moderate prime table once.  10 000 000 is sufficient for
 * evaluating the first several thousand terms. */
static void init_primes(void)
{
    if (prime_list != NULL)
        return;
    idx_t limit = 10000000;                 /* 10 M */
    build_primes_upto(limit, &prime_list, &prime_cnt);
}

/* Helper: return a bit mask of the decimal digits appearing in x
 * (x > 0).  Bits 0–9 correspond to digits 0–9. */
static unsigned digit_mask(uint64_t x)
{
    unsigned mask = 0;
    while (x > 0) {
        mask |= 1u << (x % 10);
        x /= 10;
    }
    return mask;
}

/* Main sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n=0, guard anyway */

    init_primes();             /* ensure prime table is ready */

    idx_t found   = 0;          /* how many valid numbers we have found */
    idx_t cand    = 4;          /* first composite candidate */

    for (;;) {
        /* skip primes */
        if (is_prime(cand)) {
            ++cand;
            continue;
        }

        uint64_t temp   = (uint64_t)cand; /* will be factorised */
        unsigned used_mask = 0;           /* digits already used */
        bool ok = true;

        /* trial division by pre‑computed primes */
        for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i) {
            uint64_t p = prime_list[i];
            if (temp % p == 0) {
                /* the factor appears; ensure it is not repeated */
                int exp = 0;
                do {
                    ++exp;
                    temp /= p;
                } while (temp % p == 0);
                if (exp > 1) {           /* repeated prime factor → fail */
                    ok = false;
                    break;
                }

                unsigned mask = digit_mask(p);
                if (mask & used_mask) { /* digit conflict → fail */
                    ok = false;
                    break;
                }
                used_mask |= mask;       /* record used digits */
            }
        }

        /* remaining prime factor > sqrt, if any */
        if (ok && temp > 1) {
            unsigned mask = digit_mask(temp);
            if (mask & used_mask)
                ok = false;
            else
                used_mask |= mask;
        }

        if (ok) {                  /* candidate satisfies the property */
            ++found;
            if (found == n)
                return (seq_t)cand;
        }

        ++cand;
    }
}
