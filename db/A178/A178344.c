/*
 * A178344  a(n) = Sum_i prime(i+1)^b(i) where n = Sum_{i>=0} b(i)*2^e(i) is
 *          the binary representation of n.
 * https://oeis.org/A178344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the sum of prime(i+1)^{b(i)} for n represented in binary.
 *            Here b(i) is the i-th bit of n (0 or 1). For a bit equal to 0 we
 *            add 1 (prime(i+1)^0 = 1); for a bit equal to 1 we add the
 *            corresponding prime. The highest considered bit is the position
 *            of the most significant 1 in n. By definition...
 * Range:     u8 n <= 16123, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 90 terms through n = 9747 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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
    /* a(0) is given as 1 in the OEIS data. */
    if (n == 0) return (seq_t)1;

    idx_t tmp   = n;
    idx_t bit   = 0;
    seq_t sum   = 0;

    while (tmp != 0) {
        if (tmp & 1ULL)                /* bit = 1 → add prime(bit+1) */
            sum += (seq_t)get_prime(bit + 1);
        else                           /* bit = 0 → add 1 */
            sum += (seq_t)1;
        tmp >>= 1ULL;
        ++bit;
    }
    return sum;
}
