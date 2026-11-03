/*
 * A226520  Irregular triangle read by rows: T(n,k) = Legendre(k,prime(n)),
 *          for 0 <= k < prime(n).
 * https://oeis.org/A226520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 4011, i16 n <= 12718, i32 n <= 12718, i64 n <= 12718,
 *            i128 n <= 12718, big any n
 * Validated: standard, pass, 93 terms through n = 8741 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the A226520 sequence:
   flatten the triangle T(r,k) = Legendre(k, prime(r)) for 0 ≤ k < prime(r).
   The function locates the row containing the n‑th term and returns the
   corresponding Legendre symbol. */

#include <stdint.h>
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

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

static seq_t legendre_symbol(seq_t a, uint32_t p)
{
    /* Handle the case p = 2 explicitly.  For 0≤k<2 only k=0 appears. */
    if (p == 2U) {
        uint64_t r = (uint64_t)(a >= 0 ? a : -a);
        r %= 8ULL;
        return (seq_t)((r == 1ULL || r == 7ULL) ? 1
                                               : (r == 3ULL || r == 5ULL) ? -1
                                                                          : 0);
    }

    /* For odd primes use the Jacobi symbol, which equals the Legendre symbol. */
    uint64_t aa = (uint64_t)(a >= 0 ? a : -a);
    return (seq_t)jacobi_symbol(aa, (uint64_t)p);
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1: a(1) is the first term.  For n==0 return 0. */
    if (n == 0)
        return (seq_t)0;

    idx_t target = n;      /* target position in flattened triangle */
    idx_t cum    = 0;      /* cumulative number of terms up to previous row */
    idx_t row    = 1;      /* current row index (1‑based) */

    /* Find minimal row such that cum + prime(row) >= target */
    while (true) {
        uint32_t p = get_prime(row);
        if (cum + (idx_t)p >= target)
            break;
        cum += (idx_t)p;
        ++row;
    }

    /* Column index within the found row (0‑based) */
    idx_t k = target - cum - 1;
    uint32_t p = get_prime(row);

    return legendre_symbol((seq_t)k, p);
}
