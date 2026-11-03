/*
 * A117460  Primes prime(i) such that their sum-of-index-digits A007953(i) and
 *          their sum-of-digits A007605(i) are consecutive primes.
 * https://oeis.org/A117460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the n-th prime p_i such that the digit sum of the index i
 *            (sum_digits(i)) is a prime, the digit sum of p_i is a prime, and
 *            the latter is the next prime after the former. (a(1)=2 is a
 *            special inclusion.)
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-01)
 * Helpers:   digit_sum, is_prime_seq, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_TERMS 20000

/* Utility functions provided by the test harness. */
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_seq.h"
#include "../utils/next_prime.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Sum of decimal digits of an idx_t value. */
static unsigned sum_digits_idx(idx_t x)
{
    unsigned s = 0;
    while (x != 0) {
        s += (unsigned)(x % 10);
        x /= 10;
    }
    return s;
}

/* Static cache for computed terms. */
static seq_t seq[MAX_TERMS];
static size_t seq_len = 0;
static idx_t next_index = 1;      /* next prime index to examine */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range, never used */

    if (seq_len >= (size_t)n)             /* already computed */
        return seq[n - 1];

    /* Generate terms until we have enough. */
    while (seq_len < (size_t)n) {
        /* Special inclusion of the first term 2 (index 1). */
        if (next_index == 1) {
            seq[seq_len++] = (seq_t)get_prime(1);   /* prime(1) = 2 */
            ++next_index;
            continue;
        }

        seq_t p = (seq_t)get_prime(next_index);

        unsigned sum_index = sum_digits_idx(next_index);
        seq_t sum_p_s = digit_sum(p);
        uint64_t sum_p = (uint64_t)sum_p_s;

        if (is_prime_seq((uint64_t)sum_index) &&
            is_prime_seq(sum_p) &&
            next_prime((uint64_t)sum_index) == sum_p) {

            seq[seq_len++] = p;
        }

        ++next_index;
    }

    return seq[n - 1];
}
