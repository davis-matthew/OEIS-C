/*
 * A130071  Triangle, A007444(k) in each column interspersed with k zeros.
 * https://oeis.org/A130071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 0 if c does not divide r, * otherwise a(n) = Σ_{d|c} μ(d)
 *            * p(c/d), * where r = ceil((√(8n+1)-1)/2), c = n - r(r-1)/2, * μ
 *            is the Möbius function, and p(k) is the k‑th prime
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

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

/* Binary search for smallest r such that r*(r+1)/2 >= n. */
static inline idx_t triangular_root(idx_t n)
{
    idx_t low = 1, high = n, r = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val >= n) {
            r = mid;
            if (mid == 0) break;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* n>=1 by definition, but guard anyway */

    /* Determine the row r and column c of the flattened triangle. */
    idx_t r = triangular_root(n);
    idx_t prev = (idx_t)((__uint128_t)r * (r - 1) / 2);
    idx_t c = n - prev;          /* column index (1‑based) */

    /* If the column does not divide the row, the entry is zero. */
    if (r % c != 0) return 0;

    /* Compute A007444(c) = Σ_{d|c} μ(d) * p(c/d). */
    int64_t sum = 0;
    for (idx_t d = 1; (__uint128_t)d * d <= c; ++d) {
        if (c % d == 0) {
            int mu1 = mu_of(d);
            if (mu1 != 0) {
                seq_t prime1 = get_prime(c / d);
                sum += (int64_t)mu1 * (int64_t)prime1;
            }
            idx_t d2 = c / d;
            if (d2 != d) {
                int mu2 = mu_of(d2);
                if (mu2 != 0) {
                    seq_t prime2 = get_prime(d);
                    sum += (int64_t)mu2 * (int64_t)prime2;
                }
            }
        }
    }
    return (seq_t)sum;
}
