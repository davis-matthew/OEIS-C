/*
 * A074125  Group the composite numbers so that the sum of the n-th group is a
 *          multiple of the n-th prime: (4), (6), (8, 9, 10, 12, 14, 15, 16,
 *          18, 20, 21, 22), (24, 25), (26, 27, 28, 30, 32), (33, 34, ...),
 *          ... Sequence gives the first term of each group.
 * https://oeis.org/A074125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sequentially group composite numbers so that the sum
 * of the n-th group is a multiple of the n-th prime; returns the
 * first composite of each group (a(1)=4). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

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
    if (n == 0) return 0; /* offset starts at 1, value unused */

    /* static state for memoisation */
    static bool initialized = false;
    static seq_t *firsts = NULL;
    static size_t firsts_len = 0, firsts_cap = 0;
    static seq_t curr = 0;          /* next candidate integer to consider */

    if (!initialized) {
        /* initialisation */
        firsts_cap = 64;
        firsts = mem_new(seq_t, firsts_cap);
        firsts_len = 0;
        curr = 4; /* first composite number */
        initialized = true;
    }

    while (firsts_len < n) {
        /* skip any primes to find the next composite */
        while (is_prime_seq(curr)) curr++;

        /* start a new group with current composite */
        firsts[firsts_len++] = curr;

        /* grow array if necessary */
        if (firsts_len > firsts_cap) {
            firsts_cap <<= 1;
            firsts = mem_resize(seq_t, firsts, firsts_cap);
        }

        /* prime for this group (1‑based index) */
        seq_t prime = (seq_t)get_prime((idx_t)firsts_len);

        /* accumulate sum until divisible by the prime */
        seq_t sum = 0;
        seq_t c = curr;
        do {
            sum += c;
            c++;
            while (is_prime_seq(c)) c++; /* skip primes inside the group */
        } while (sum % prime != 0);

        /* next group starts at next composite after the last added */
        curr = c;
    }

    return firsts[n - 1];
}
