/*
 * A079900  a(n) = the smallest positive number which furnishes a "one-line
 *          proof" for primality of prime(n), the n-th prime; i.e., the
 *          smallest k which is relatively prime to p such that k*(p+k) is
 *          divisible by every prime less than sqrt(p), where p=prime(n).
 * https://oeis.org/A079900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained by searching the smallest integer k such that *
 *            - k is relatively prime to p = prime(n) * - for every prime q <
 *            sqrt(p) either q | k or q | (p + k) * i.e. for each such q, k %
 *            q is either 0 or q - (p % q)
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
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

static inline idx_t isqrt_u64(idx_t n)
{
    /* integer square root, for n >= 0 */
    if (n < 2) return n;
    idx_t lo = 1, hi = n, ans = 1;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t div = n / mid;
        if (mid == div)          /* mid*mid == n */
            return mid;
        if (mid < div) {
            ans = mid;
            lo  = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Prime p = prime(n) ------------------------------------------- */
    seq_t p = (seq_t)get_prime(n);

    /* Primes up to sqrt(p) ----------------------------------------- */
    idx_t limit   = isqrt_u64((idx_t)p);
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;

    if (limit >= 2) {
        /* Sieve of Eratosthenes up to 'limit' */
        uint8_t *is_comp = (uint8_t *)calloc((size_t)limit + 1, 1);
        primes = (uint64_t *)malloc(((size_t)limit + 1) * sizeof(uint64_t));
        for (idx_t i = 2; i <= limit; ++i) {
            if (!is_comp[(size_t)i]) {
                primes[primes_cnt++] = (uint64_t)i;
                if (i * i <= limit)
                    for (idx_t j = i * i; j <= limit; j += i)
                        is_comp[(size_t)j] = 1;
            }
        }
        free(is_comp);
    }

    /* Search the minimal k ------------------------------------------ */
    seq_t k = (seq_t)1;
    for ( ; ; ++k) {
        if (k % p == 0) continue;                /* not coprime to p */

        bool ok = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            seq_t q   = (seq_t)primes[i];
            seq_t rem = k % q;
            seq_t remP = p % q;
            seq_t target = q - remP;             /* q - p mod q */
            if (rem != 0 && rem != target) {
                ok = false;
                break;
            }
        }
        if (ok) {
            free(primes);
            return k;
        }
    }
}
