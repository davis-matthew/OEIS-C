/*
 * A227897  Numbers k such that k^2 + 2 is not squarefree.
 * https://oeis.org/A227897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A227897: Numbers k such that k^2 + 2 is not squarefree
   Implements a lazy sieve: generate k in increasing order, store the
   sequence elements in a static array so that calls to a(n) in any order
   are efficient.
   The test function has_square_factor tests whether a 64‑bit integer
   contains a squared prime divisor by trial division with a cache of
   primes. The sequence is built up to the requested index n, and the
   required term is returned.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- integer square root of a 64‑bit number ---------- */
static uint64_t _floor_isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (mid != 0 && mid > n / mid)   /* mid*mid > n  */
            hi = mid - 1;
        else {
            if (mid * mid == n) return mid;
            lo = mid + 1;
        }
    }
    return hi;
}

/* ---------- prime sieve ------------------------------------------------ *\

   The prime table is built up to the maximum prime needed for the
   square‑factor tests.  Only uint64_t primes are stored; the table grows
   lazily.
 */
static uint64_t *primes      = NULL;   /* array of primes  */
static size_t    primes_n    = 0;      /* current number of primes   */
static size_t    primes_cap  = 0;      /* allocated capacity          */

static void _add_prime(uint64_t p)
{
    if (primes_cap == primes_n) {
        primes_cap = primes_cap ? primes_cap * 2 : 16;
        primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
    }
    primes[primes_n++] = p;
}

static void _extend_primes_to(uint64_t limit)
{
    if (primes_n == 0) _add_prime(2);
    if (primes[primes_n - 1] >= limit) return;

    uint64_t cand = primes[primes_n - 1] + 1;
    if (cand <= 2) cand = 3;
    if ((cand & 1) == 0) cand++;            /* start with an odd candidate */

    for ( ; cand <= limit; cand += 2) {
        bool isprime = true;
        uint64_t root = _floor_isqrt_u64(cand);
        for (size_t i = 0; i < primes_n && primes[i] <= root; ++i) {
            if (cand % primes[i] == 0) { isprime = false; break; }
        }
        if (isprime) _add_prime(cand);
    }
}

/* ---------- test whether n has a squared prime divisor ---------- */
static bool _has_square_factor(uint64_t n)
{
    if (n <= 3) return false;           /* 1,2,3 are squarefree        */

    uint64_t root = _floor_isqrt_u64(n);
    _extend_primes_to(root);

    for (size_t i = 0; i < primes_n && primes[i] <= root; ++i) {
        uint64_t p = primes[i];
        int cnt = 0;
        while (n % p == 0) {
            n /= p;
            if (++cnt == 2) return true;      /* found p² | original n   */
        }
        if (n == 1) break;
        root = _floor_isqrt_u64(n);           /* adjust upper bound     */
    }
    return false;
}

/* ---------- sequence storage --------------------------------------- */
static seq_t   *seq      = NULL;   /* array of found terms */
static size_t   seq_n    = 0;      /* number of terms stored    */
static size_t   seq_cap  = 0;      /* allocated capacity        */
static uint64_t next_k   = 1;      /* next k to test             */

static void _ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    seq_cap = need + 16;
    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
}

/* Find the next k such that k^2 + 2 contains a squared prime divisor
   and store it in the sequence array. */
static void _find_and_store_next(void)
{
    while (true) {
        ++next_k;
        uint64_t km = (uint64_t)next_k;
        uint64_t val = (uint64_t)(((__uint128_t)km * km) + 2ULL);
        if (_has_square_factor(val)) {
            _ensure_seq_cap(seq_n + 1);
            seq[seq_n++] = (seq_t)next_k;
            return;
        }
    }
}

/* ---------- public function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for index 0   */
    while (seq_n < n) _find_and_store_next();
    return seq[n - 1];
}
