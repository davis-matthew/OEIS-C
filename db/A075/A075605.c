/*
 * A075605  Smallest k not a palindrome and not divisible by 10 such that k
 *          and R(k) (A004086) both are divisible by the n-th prime.
 * https://oeis.org/A075605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-03)
 * Helpers:   mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A075605
   For a given n ≥ 1 let p = the n‑th prime.
   a(n) is the smallest integer k that satisfies:
      – k is not a palindrome (k ≠ rev(k))
      – k is not divisible by 10
      – both k and rev(k) are divisible by p
   The function memoises already computed terms for efficiency. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/reverse_digits.h"
#include "../utils/mem.h"

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

/* Compute the n‑th term. n is 1‑based. */
static seq_t compute_term(idx_t n)
{
    uint32_t pn = get_prime(n);      /* n‑th prime (small enough for uint32_t) */
    seq_t   p  = (seq_t)pn;          /* promote to seq_t */

    /* Search multiples of p */
    for (idx_t m = 1; ; ++m) {
        seq_t k = p * (seq_t)m;      /* candidate k */

        if ((k % 10) == 0)          /* skip multiples of 10 */
            continue;

        seq_t rev = reverse_digits(k);
        if (rev == k)               /* skip palindromes */
            continue;

        if (rev % p == 0)           /* both k and rev divisible by p */
            return k;
    }
}

/* Memoised cache of computed terms */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0)                     /* safety for out‑of‑range input */
        return 0;

    if (n > cache_size) {
        cache = mem_resize(seq_t, cache, (n + 1));   /* cast ensures C++ compatibility */
        for (idx_t i = cache_size + 1; i <= n; ++i)
            cache[i] = compute_term(i);
        cache_size = n;
    }
    return cache[n];
}
