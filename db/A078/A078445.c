/*
 * A078445  Primes in A060620, i.e., primes which are integer parts of
 *          averages of initial primes.
 * https://oeis.org/A078445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(sum_{i=1..k} prime(i)/k) for all k with this floor
 *            being prime. This implementation generates terms in ascending
 *            order by scanning k from 1 upward and recording each floor
 *            average that is prime. Each call computes the n‑th term from
 *            scratch; the cost is O(n log n), which is well belo...
 * Range:     u8 n <= 5, u16 n <= 29, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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
    if (n == 0) return 0;                  /* undefined for n < 1, 0 is a safe sentinel */

    idx_t k = 0;                           /* current number of primes processed */
    seq_t sum = 0;                         /* sum of the first k primes */
    idx_t found = 0;                       /* number of primes found in the sequence */

    while (found < n) {
        ++k;
        uint32_t p = get_prime(k);         /* k‑th prime */
        sum += (seq_t)p;                   /* update sum */

        seq_t avg = sum / (seq_t)k;        /* floor average */
        if (is_prime((idx_t)avg)) {        /* check if the floor average is prime */
            ++found;
            if (found == n) return avg;
        }
    }

    return 0; /* unreachable, but keeps the compiler happy */
}
