/*
 * A180489  Smallest pandigital number (A171102) divisible by the n-th prime
 *          A000040(n).
 * https://oeis.org/A180489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 7737, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 34 terms through n = 9956 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Algorithm: a(n) is the smallest pandigital number
 * (containing every decimal digit 0‑9 at least once) that is
 * divisible by the nth prime.
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

/* Test whether x contains all decimal digits at least once. */
static inline int is_pandigital(seq_t x)
{
    unsigned char cnt[10];
    for (int i = 0; i < 10; ++i) cnt[i] = 0;
    while (x > 0) {
        int d = (int)(x % 10);
        cnt[d]++;
        x /= 10;
    }
    for (int i = 0; i < 10; ++i) {
        if (cnt[i] == 0) return 0;
    }
    return 1;
}

/* Return the smallest pandigital multiple of the nth prime. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1 */
    const seq_t START = 1023456789;           /* smallest pandigital number */
    uint32_t prime = get_prime(n);            /* nth prime (uint32_t OK) */

    /* Start from the first multiple of prime that is >= START */
    seq_t k = START / (seq_t)prime;
    if ((seq_t)prime * k < START) ++k;

    for (;;) {
        seq_t cand = (seq_t)prime * k;
        if (is_pandigital(cand)) return cand;
        ++k;
    }
}
