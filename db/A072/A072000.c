/*
 * A072000  Number of semiprimes (A001358) <= n.
 * https://oeis.org/A072000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of integers k ≤ n such that bigomega(k) = 2 (i.e.,
 *            the number of semiprimes ≤ n, including squares of primes). This
 *            code sieves primes up to n on each call and counts unordered
 *            prime pairs p·q ≤ n, where p ≤ q
 * Range:     u8 n <= 848, u16 n <= 9337, u32 n <= 9337, u64 n <= 9337,
 *            u128 n <= 9337, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Helper: integer square root (floor of sqrt(n)) for 64‑bit unsigned */
static inline idx_t isqrt(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        /* avoid overflow: check mid > n/mid instead of mid*mid > n */
        if (mid != 0 && mid > n / mid) {
            hi = mid - 1;
        } else if (mid * mid > n) {
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return hi;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;

    /* Sieve of Eratosthenes up to n */
    bool *is_prime = (bool*)malloc((n + 1) * sizeof(bool));
    if (!is_prime) return 0;          /* error handling not required for test harness */

    for (idx_t i = 0; i <= n; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; (i * i) <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* collect primes */
    idx_t pi = 0;
    for (idx_t i = 2; i <= n; ++i) if (is_prime[i]) ++pi;
    idx_t *primes = (idx_t*)malloc(pi * sizeof(idx_t));
    if (!primes) { free(is_prime); return 0; }
    idx_t pidx = 0;
    for (idx_t i = 2; i <= n; ++i) if (is_prime[i]) primes[pidx++] = i;

    /* count semiprimes */
    seq_t count = 0;
    idx_t sqrt_n = isqrt(n);
    for (idx_t i = 0; i < pi && primes[i] <= sqrt_n; ++i) {
        idx_t p = primes[i];
        idx_t limit = n / p;          /* maximal q with p*q <= n */
        /* binary search for upper bound of primes <= limit */
        idx_t lo = i, hi = pi;
        while (lo < hi) {
            idx_t mid = lo + ((hi - lo + 1) >> 1);
            if (primes[mid - 1] <= limit) lo = mid;
            else hi = mid - 1;
        }
        idx_t qcnt = lo - i;          /* number of primes q with q >= p and p*q <= n */
        count += qcnt;
    }

    free(primes);
    free(is_prime);
    return count;
}
