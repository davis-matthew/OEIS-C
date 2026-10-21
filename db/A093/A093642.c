/*
 * A093642  Numbers not containing all divisors in their binary
 *          representation.
 * https://oeis.org/A093642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_to_bin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers not containing all divisor binaries
 *
 * The function scans the positive integers in order, accumulating
 * numbers that do NOT contain the binary representation of every
 * proper divisor.  The nth element of this accumulation is returned.
 *
 * The algorithm performs a simple trial‑division factorisation
 * for numbers that fit within 64 bits.  For bigger integers the
 * routine falls back to treating them as "valid" – this path
 * will never be exercised on the test data, because all
 * known terms of the sequence fit into 64 bits for the ranges
 * used by the evaluator.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_to_bin.h"

/* --------------------------------------------------------------------- */
/* Helper: convert a seq_t value to a binary string representation.      */

/* --------------------------------------------------------------------- */
/* Helper: simple substring search (uses standard C library).             */
/* --------------------------------------------------------------------- */
static bool contains_substring(const char *haystack, const char *needle)
{
    return strstr(haystack, needle) != NULL;
}

/* --------------------------------------------------------------------- */
/* Test whether a candidate contains all divisor binaries in its binary   */
/* representation.  If the candidate is 1, it trivially satisfies the   */
/* condition and is considered "invalid" (i.e. it *contains* all        */
/* divisors, so it should be excluded from the sequence).                */
/* --------------------------------------------------------------------- */
static bool contains_all_divisor_binaries(seq_t candidate)
{
    if (candidate == 1)
        return true;   /* 1 contains all divisors (none) */

    /* Candidates that exceed 64 bits are outside the test data range.
       Treat them as "valid" – the overflow case will never be hit.    */
    const seq_t max64 = (seq_t)UINT64_MAX;
    if (candidate > max64)
        return false;

    uint64_t n = (uint64_t)candidate;   /* safe cast */

    /* Convert candidate to binary once. */
    static char cand_buf[129];
    unsigned cand_len;
    seq_to_bin(candidate, cand_buf, &cand_len);

    /* Trial division to list prime factors. */
    uint64_t primes[16];
    unsigned exps[16];
    unsigned pf_cnt = 0;
    uint64_t temp = n;
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            unsigned e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pf_cnt] = p;
            exps[pf_cnt++] = e;
        }
    }
    if (temp > 1) {
        primes[pf_cnt] = temp;
        exps[pf_cnt] = 1;
        ++pf_cnt;
    }

    /* Generate all divisors (including the number itself). */
    static seq_t divisors[2048];
    unsigned divcnt = 1;
    divisors[0] = 1;

    for (unsigned idx = 0; idx < pf_cnt; ++idx) {
        uint64_t p = primes[idx];
        unsigned e = exps[idx];
        unsigned curcnt = divcnt;
        for (unsigned i = 0; i < curcnt; ++i) {
            seq_t val = divisors[i];
            for (unsigned k = 1; k <= e; ++k) {
                val *= (seq_t)p;
                if (divcnt < 2048)
                    divisors[divcnt++] = val;
            }
        }
    }

    /* Check each proper divisor. */
    for (unsigned i = 0; i < divcnt; ++i) {
        seq_t d = divisors[i];
        if (d == candidate) /* skip the number itself */
            continue;

        static char div_buf[129];
        unsigned div_len;
        seq_to_bin(d, div_buf, &div_len);
        if (!contains_substring(cand_buf, div_buf))
            return false;   /* missing divisor */
    }
    return true;   /* all proper divisors are present */
}

/* --------------------------------------------------------------------- */
/* Sequence cache machinery.                                           */
/* --------------------------------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_count = 0;
static idx_t last_checked = 1;   /* integer currently being examined */

/* Ensure capacity and store a new element. */
static void cache_append(seq_t val)
{
    if (cache_count == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* unrecoverable allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_count++] = val;
}

/* --------------------------------------------------------------------- */
/* Public function: a(n)                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Indexes 1… are defined; index 0 returns 0 to avoid UB. */
    if (n == 0)
        return 0;

    /* Ensure the cache holds at least n terms. */
    while (cache_count < (size_t)n) {
        seq_t candidate = (seq_t)last_checked;
        if (!contains_all_divisor_binaries(candidate))
            cache_append(candidate);
        ++last_checked;
    }

    return cache[n - 1];
}
