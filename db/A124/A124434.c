/*
 * A124434  LCM (least common multiple) of A001043 (sum of consecutive primes)
 *          and A001223 (difference of consecutive primes).
 * https://oeis.org/A124434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = lcm(prime(n+1)+prime(n), prime(n+1)-prime(n)) * =
 *            (prime(n+1)^2 − prime(n)^2)/2 for n > 1 * a(1) = 5
 * Range:     u8 n <= 14, u16 n <= 428, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 8810 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

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

static seq_t lcm_seq(seq_t x, seq_t y)
{
    /* Compute lcm(x,y) = x/gcd(x,y)*y */
    return (x / gcd(x, y)) * y;
}

seq_t a(idx_t n)
{
    /* a(1) is a special case because the closed form would give 2.5 */
    if (n == 1) {
        return (seq_t)5;
    }

    /* Retrieve the nth and (n+1)th primes (sequence is 1‑indexed) */
    seq_t p_n     = (seq_t)get_prime(n);
    seq_t p_next  = (seq_t)get_prime(n + 1);

    /* Sum and difference of consecutive primes */
    seq_t sum  = p_n + p_next;
    seq_t diff = p_next - p_n;

    /* lcm of the sum and difference gives the desired term */
    return lcm_seq(sum, diff);
}
