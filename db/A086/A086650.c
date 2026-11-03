/*
 * A086650  n-th composite number raised to the n-th prime number.
 * https://oeis.org/A086650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A002808(n)^A000040(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-01)
 * Helpers:   ipow, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/ipow.h"
#include "../utils/is_prime.h"

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

seq_t a(idx_t n) {
    /* a(1) is the first term, values for n == 0 are unused */
    if (n == 0) return (seq_t)0;

    /* static cache for composites */
    static idx_t *composites = NULL;
    static size_t comp_cap  = 0;
    static idx_t comp_cnt   = 0;
    static idx_t next_cand  = 4;                /* smallest composite */

    /* grow composite cache as needed */
    if (comp_cnt < n) {
        /* ensure capacity for new entries */
        if (comp_cap < comp_cnt + (size_t)(n - comp_cnt) + 10) {
            size_t new_cap = comp_cap ? comp_cap * 2 : 16;
            while (new_cap < comp_cnt + (size_t)(n - comp_cnt) + 10)
                new_cap *= 2;
            idx_t *tmp = (idx_t *)realloc(composites, new_cap * sizeof(idx_t));
            if (!tmp) abort();                    /* allocation failure */
            composites = tmp;
            comp_cap = new_cap;
        }

        /* find composites until we have at least n of them */
        while (comp_cnt < n) {
            while (is_prime(next_cand))
                ++next_cand;
            composites[comp_cnt++] = next_cand;
            ++next_cand;
        }
    }

    idx_t base_idx  = composites[n - 1];
    idx_t prime_idx = get_prime(n);          /* nth prime (idx_t) */

    return ipow((seq_t)base_idx, (seq_t)prime_idx);
}
