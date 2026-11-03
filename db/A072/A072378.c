/*
 * A072378  Numbers n such that 12*n divides F(12*n), where F(m) is the m-th
 *          Fibonacci number.
 * https://oeis.org/A072378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number k such that 12*k | Fib(12*k) Computed by search
 *            with fast doubling for Fibonacci modulo m (m = 12*k). The first
 *            24 terms are:
 *            1,2,3,4,5,6,8,9,10,12,14,15,16,18,20,24,25,27,28,30,32,36,40,42
 * Range:     u8 n <= 88, u16 n <= 2795, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9310 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Fast doubling algorithm for Fibonacci modulo `mod`.
   Returns F(n) mod mod. All intermediate arithmetic fits in
   64‑bit because n (and thus mod) stays below a few million. */
static inline void fib_pair_uint64(uint64_t n, uint64_t *fn, uint64_t *fn1, uint64_t mod)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    uint64_t a, b;
    fib_pair_uint64(n >> 1, &a, &b, mod);
    /* c = F(2k) = F(k) * (2*F(k+1) – F(k)) */
    uint64_t two_b = (b << 1) % mod;
    uint64_t diff = (two_b >= a) ? (two_b - a) : (two_b + mod - a);
    uint64_t c = (a * diff) % mod;

    /* d = F(2k+1) = F(k)^2 + F(k+1)^2 */
    uint64_t d = ((a * a) % mod + (b * b) % mod) % mod;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

static inline uint64_t fib_mod_uint64(uint64_t n, uint64_t mod)
{
    uint64_t fn, fn1;
    fib_pair_uint64(n, &fn, &fn1, mod);
    return fn;
}

/* Check whether 12*k divides Fib(12*k). */
static inline bool good_candidate(uint64_t k)
{
    uint64_t mod = k * 12ULL;            /* m = 12*k  */
    return fib_mod_uint64(mod, mod) == 0ULL;
}

/* Public function: index n (≥1) → n‑th term of seq A072378. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, but harmless */
    uint64_t need = (uint64_t)n;
    uint64_t found = 0;
    uint64_t cand   = 1;

    while (true) {
        if (good_candidate(cand)) {
            ++found;
            if (found == need)
                return (seq_t)cand;
        }
        ++cand;
    }
}
