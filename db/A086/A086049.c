/*
 * A086049  Position of first occurrence of n-th prime in concatenated primes
 *          (A033308, Copeland-Erdos constant).
 * https://oeis.org/A086049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) equals the 1‑based position of the first occurrence of the
 *            n-th prime in the concatenated prime string (Copeland–Erdős
 *            constant concatenation), plus 1
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 8226 (2026-11-01)
 * Helpers:   num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/num_digits0.h"

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

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Compute the total number of digits of all primes up to n. */
    size_t total_digits = 0;
    for (idx_t i = 1; i <= n; ++i) {
        total_digits += (size_t)num_digits0(get_prime(i));
    }

    /* Allocate the concatenated string and build it. */
    char *S = (char *)malloc(total_digits + 1);
    if (!S) return (seq_t)0;   /* out of memory */

    size_t pos = 0;
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t p = get_prime(i);
        /* Append decimal representation of p to S. */
        int len = sprintf(S + pos, "%llu", (unsigned long long)p);
        pos += (size_t)len;
    }
    S[total_digits] = '\0';

    /* Build the string for the target prime. */
    uint64_t target = get_prime(n);
    char target_str[32];
    int target_len = sprintf(target_str, "%llu", (unsigned long long)target);
    target_str[target_len] = '\0';

    /* Find the first occurrence. */
    char *found = strstr(S, target_str);
    seq_t result = 0;
    if (found) {
        /* +2: convert 0‑based offset to 1‑based position and add 1. */
        result = (seq_t)(found - S + 2);
    }

    free(S);
    return result;
}
