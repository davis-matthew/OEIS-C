/*
 * A108110  Numbers n such that prime(k)*n+prime(k+1), for k=1,...,6 all are
 *          primes.
 * https://oeis.org/A108110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find all integers n such that
   prime(k)*n + prime(k+1) is prime for k = 1..6.
   The first few terms are: 284, 3074, 3494, ...  */

/*  Helper: deterministic Miller–Rabin for 64‑bit integers.
    Works for the 12 fixed bases that make the test deterministic.   */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static inline uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)(((__uint128_t)a * b) % m);
}

static inline uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mod_mul(r, a, m);
        a = mod_mul(a, a, m);
        e >>= 1;
    }
    return r;
}

static bool miller_rabin_base(uint64_t d, uint64_t n, uint64_t a)
{
    if (a % n == 0) return true;
    uint64_t x = mod_pow(a, d, n);
    if (x == 1 || x == n - 1) return true;
    uint64_t t = d;
    while (t != n - 1) {
        x = mod_mul(x, x, n);
        t <<= 1;
        if (x == n - 1) return true;
    }
    return false;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small) / sizeof(small[0]); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    uint64_t d = n - 1;
    while ((d & 1) == 0) d >>= 1;
    /* 12 deterministic bases for <2^64  */
    return miller_rabin_base(d, n, 2) &&
           miller_rabin_base(d, n, 3) &&
           miller_rabin_base(d, n, 5) &&
           miller_rabin_base(d, n, 7) &&
           miller_rabin_base(d, n, 11) &&
           miller_rabin_base(d, n, 13) &&
           miller_rabin_base(d, n, 17) &&
           miller_rabin_base(d, n, 19) &&
           miller_rabin_base(d, n, 23) &&
           miller_rabin_base(d, n, 29) &&
           miller_rabin_base(d, n, 31) &&
           miller_rabin_base(d, n, 37);
}

/* Test whether a candidate value n satisfies the six primality conditions. */
static inline bool candidate_ok(uint64_t n)
{
    return   is_prime_uint64((uint64_t)(((__uint128_t)2 * n + 3))) &&
            is_prime_uint64((uint64_t)(((__uint128_t)3 * n + 5))) &&
            is_prime_uint64((uint64_t)(((__uint128_t)5 * n + 7))) &&
            is_prime_uint64((uint64_t)(((__uint128_t)7 * n + 11))) &&
            is_prime_uint64((uint64_t)(((__uint128_t)11 * n + 13))) &&
            is_prime_uint64((uint64_t)(((__uint128_t)13 * n + 17)));
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but harmless */

    /* static storage for computed terms */
    static seq_t *seq   = NULL;
    static idx_t cap    = 0;
    static idx_t sz     = 0;
    static uint64_t next_k = 0;     /* next k to test (0‑based) */

    if (sz < n) {
        idx_t needed = n;
        if (cap < needed) {
            if (cap == 0) cap = 64;
            while (cap < needed) cap <<= 1;
            seq = (seq_t *)realloc(seq, sizeof(seq_t) * cap);
        }

        /* Generate more terms until we have n elements.  */
        while (sz < n) {
            for (uint64_t k = next_k; ; ++k) {
                uint64_t cand1 = 8 + 30ULL * k;     /* n ≡ 8 (mod 30)  */
                uint64_t cand2 = 14 + 30ULL * k;    /* n ≡14 (mod 30)  */

                if (candidate_ok(cand1)) {
                    seq[sz++] = (seq_t)cand1;
                    next_k = k + 1;
                    break;
                }
                if (candidate_ok(cand2)) {
                    seq[sz++] = (seq_t)cand2;
                    next_k = k + 1;
                    break;
                }
            }
        }
    }
    return seq[n - 1];
}
