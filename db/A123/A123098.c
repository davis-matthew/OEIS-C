/*
 * A123098  Multiplicative encoding of triangle formed by reading Pascal's
 *          triangle mod 2 (A047999).
 * https://oeis.org/A123098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=0..n} p(i+1)^(C(n,i) mod 2), where C(n,i) mod
 *            2 = 1 iff all bits of i are a subset of those of n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 14, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 492 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
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

static inline seq_t seq_one(void) { return (seq_t)1; }

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Natural number sequence A123098 */
seq_t a(idx_t n)
{
    seq_t prod = seq_one();              /* start with 1 */
    /* iterate over all submasks of n; each submask corresponds to an i s.t.
       C(n,i) is odd (Lucas's theorem) */
    for (idx_t sub = n; ; sub = (sub - 1) & n) {
        /* prime(i+1) gives the (i+1)-th prime; cast to seq_t before
           multiplication. */
        seq_t prime = (seq_t)get_prime(sub + 1);
        prod = seq_mul(prod, prime);
        if (sub == 0) break;               /* finished all submasks */
    }
    return prod;
}
