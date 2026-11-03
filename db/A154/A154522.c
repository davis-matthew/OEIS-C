/*
 * A154522  Numbers n such that number of occurrences of the smallest digit in
 *          prime(n) is 2.
 * https://oeis.org/A154522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th index k such that the smallest digit in prime(k)
 *            appears exactly twice. The function keeps a memoised table of
 *            all indices found so far
 * Range:     u8 n <= 11, u16 n <= 1735, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 8154 (2026-11-01)
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/digit_frequencies.h"

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

static idx_t *qual = NULL;   /* array of qualifying indices (prime indices) */
static size_t qual_len = 0;  /* number of stored qualifying indices      */
static size_t qual_cap = 0;  /* allocated capacity of qual array         */
static idx_t last_checked = 0; /* last prime index that has been examined */

static void ensure_capacity(void **ptr, size_t *cap, size_t needed, size_t elem_size)
{
    if (*cap < needed) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        void *tmp = realloc(*ptr, new_cap * elem_size);
        if (!tmp) abort();
        *ptr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n=0, safe to return 0 */

    /* generate qualifying indices until we have at least n of them */
    while (qual_len < n) {
        idx_t next_idx = last_checked + 1;
        uint32_t prime_val = get_prime(next_idx);

        unsigned int freq[10] = {0};
        digit_frequencies((seq_t)prime_val, freq);

        int smallest = -1;
        for (int d = 0; d <= 9; ++d) {
            if (freq[d] > 0) { smallest = d; break; }
        }

        if (smallest >= 0 && freq[smallest] == 2) {
            ensure_capacity((void**)&qual, &qual_cap, qual_len + 1, sizeof(idx_t));
            qual[qual_len++] = next_idx;
        }

        last_checked = next_idx;
    }

    return (seq_t)qual[n - 1];
}
