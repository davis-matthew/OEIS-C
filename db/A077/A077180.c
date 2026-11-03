/*
 * A077180  Smallest concatenation of integers starting from 1 that is a
 *          multiple of the n-th prime.
 * https://oeis.org/A077180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-01)
 * Helpers:   num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: smallest integer formed by concatenating 1,2,...,m such that
   it is a multiple of the n‑th prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <math.h>
#include <stddef.h>
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

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

/* Compute a(n) for OEIS sequence A077180. */
seq_t a(idx_t n)
{
    /* Undefined for offset 1 but harmless. */
    if (n == 0)
        return 0;

    /* The required prime.  The helper returns it as idx_t but
       we know it fits in 64 bits. */
    uint64_t prime = (uint64_t)get_prime(n);

    /* Working modulo the prime. */
    uint64_t mod = 0;

    /* Temporary buffer to build the decimal representation. */
    size_t buf_cap = 1024;
    size_t buf_len = 0;
    char *buf = (char *)malloc(buf_cap);
    if (!buf)
        return 0;   /* allocation failure */

    /* Loop over consecutive integers to find the first divisible
       concatenation. */
    for (uint64_t k = 1; ; ++k)
    {
        /* Number of decimal digits of k. */
        uint64_t digits = num_digits0(k);

        /* Compute 10^digits mod prime. */
        uint64_t pow10mod = 1 % prime;
        for (uint64_t i = 0; i < digits; ++i)
            pow10mod = (pow10mod * 10ULL) % prime;

        /* Update the modulo value for the new concatenation. */
        mod = (mod * pow10mod + k) % prime;

        /* Append k to the buffer as decimal text. */
        int written = snprintf(buf + buf_len, buf_cap - buf_len,
                               "%llu", (unsigned long long)k);
        if (written < 0) {
            free(buf);
            return 0;   /* encoding error */
        }
        buf_len += (size_t)written;

        /* Enlarge the buffer if its remaining space is too small. */
        if (buf_cap - buf_len < 64) {
            size_t new_cap = buf_cap * 2;
            char *tmp = (char *)realloc(buf, new_cap);
            if (!tmp) {
                free(buf);
                return 0;   /* reallocation failure */
            }
            buf = tmp;
            buf_cap = new_cap;
        }

        /* Stop once we hit a concatenation divisible by the prime. */
        if (mod == 0)
            break;
    }

    /* Convert the final concatenated decimal string to seq_t. */
    seq_t result = seq_from_str(buf);
    free(buf);
    return result;
}
