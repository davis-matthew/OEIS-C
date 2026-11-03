/*
 * A166745  a(n) is the concatenation, in ascending order, of the set of
 *          digits 1,2,4,8 whose sum equals the n-th prime using a minimal
 *          number of digits.
 * https://oeis.org/A166745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   For the n‑th prime p, express p as a sum of the fewest digits
 *            from the set {1,2,4,8}. The resulting digits are concatenated in
 *            ascending order
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

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
    /* Get the n‑th prime (1‑based). */
    uint32_t prime = get_prime(n);

    /* Greedy decomposition – optimal for powers of two. */
    idx_t cnt8 = (idx_t)(prime / 8);
    uint64_t r = (uint64_t)prime % 8;
    idx_t cnt4 = (idx_t)(r / 4);
    r %= 4;
    idx_t cnt2 = (idx_t)(r / 2);
    idx_t cnt1 = (idx_t)(r % 2);

    /* Build the concatenated number in ascending digit order. */
    seq_t result = 0;
    for (idx_t i = 0; i < cnt1; ++i) result = result * 10 + 1;
    for (idx_t i = 0; i < cnt2; ++i) result = result * 10 + 2;
    for (idx_t i = 0; i < cnt4; ++i) result = result * 10 + 4;
    for (idx_t i = 0; i < cnt8; ++i) result = result * 10 + 8;
    return result;
}
