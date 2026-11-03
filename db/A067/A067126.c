/*
 * A067126  Numbers for which phi(n) >= phi(k) for all k = 1 to n-1.
 * https://oeis.org/A067126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n‑th integer whose Euler totient φ(n) is a record *
 *            (≥ all previous φ(k), 1 ≤ k < n). The sequence coincides with
 *            primes * after the term 9, with the following exception: *
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5, a(6)=7, a(7)=9, and for
 *            n ≥ 8 * a(n) = the (n‑3)‑rd prime
 * Range:     u8 n <= 57, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
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
    /* Handle the first few terms explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)4;
    if (n == 5) return (seq_t)5;
    if (n == 6) return (seq_t)7;
    if (n == 7) return (seq_t)9;

    /* For n ≥ 8, the sequence consists of primes:
     * a(n) = prime(n-3)   (1‑based indexing of primes) */
    return get_prime(n - 3);
}
