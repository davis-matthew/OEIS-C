/*
 * A074199  a(n) = n!/(1!*2!*3!*...*k!) where k is the largest integer such
 *          that 1!*2!*3!*...*k! divides n!.
 * https://oeis.org/A074199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! / (1!·2!·3!·...·k!) where k is the largest integer
 *            such that (1!·2!·...·k!) divides n!. Computation is performed
 *            via prime‐factor exponents
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 21, u64 n <= 35, u128 n <= 55,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 720 (2026-11-01)
 * Helpers:   build_primes_upto, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Exponent of prime p in n! (Legendre's formula). */
static inline seq_t _fac_exp(idx_t n, uint64_t p)
{
    seq_t e = 0;
    for (idx_t div = (idx_t)p; div <= n; ) {
        e += n / div;
        if (div > n / p) break;
        div *= p;
    }
    return e;
}

/* Exponent of prime p in superfactorial(k) = ∏_{i=1}^k i!. */
static inline seq_t _super_exp(idx_t k, uint64_t p)
{
    seq_t e = 0;
    for (idx_t div = (idx_t)p; div <= k; ) {
        idx_t m = k / div;
        seq_t term1 = (seq_t)div * (seq_t)m * (seq_t)(m-1) / 2;
        seq_t term2 = (seq_t)m * (seq_t)(k - m*div + 1);
        e += term1 + term2;
        if (div > k / p) break;
        div *= p;
    }
    return e;
}

/* static storage for primes up to the maximum n seen */
static uint64_t * _primes = NULL;
static size_t    _prime_cnt = 0;
static idx_t     _prime_limit = 0;

/* Ensure primes up to 'limit' are available. */
static void _ensure_primes(idx_t limit)
{
    if (limit <= _prime_limit) return;
    uint64_t *newp = NULL;
    size_t newc = 0;
    build_primes_upto(limit, &newp, &newc);
    if (_primes) free(_primes);
    _primes = newp;
    _prime_cnt = newc;
    _prime_limit = limit;
}

/* Does ∏_{i=1}^k i! divide n! ?  All primes ≤ n are checked. */
static bool _super_dvd_fact(idx_t k, idx_t n)
{
    for (size_t i = 0; i < _prime_cnt; ++i) {
        uint64_t p = _primes[i];
        if (p > n) break;
        seq_t ep_n = _fac_exp(n, p);
        seq_t ep_k = _super_exp(k, p);
        if (ep_k > ep_n) return false;
    }
    return true;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* Offset starts at 1 – undefined below 1 */
    _ensure_primes(n);

    /* Binary search for the maximal k with superfactorial(k) | n!. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (_super_dvd_fact(mid, n))
            low = mid;
        else
            high = mid - 1;
    }
    idx_t k = low;

    /* Compute a(n) = n! / superfactorial(k) via prime exponents. */
    seq_t res = 1;
    for (size_t i = 0; i < _prime_cnt; ++i) {
        uint64_t p = _primes[i];
        if (p > n) break;
        seq_t ep_n = _fac_exp(n, p);
        seq_t ep_k = _super_exp(k, p);
        seq_t diff = ep_n - ep_k;
        if (diff) {
            seq_t powp = ipow((seq_t)p, diff);
            res = seq_mul(res, powp);
        }
    }
    return res;
}
