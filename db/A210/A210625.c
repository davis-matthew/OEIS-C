/*
 * A210625  Least semiprime dividing digit reversal of n, or 0 if no such
 *          factor.
 * https://oeis.org/A210625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = least semiprime divisor of rev(n) (or 0 if none)
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses trial‑division factorization of rev(n) */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/rev.h"

seq_t a(idx_t n)
{
    /* reverse the decimal representation of n */
    idx_t rev_n = rev(n);
    if (rev_n <= 1) return 0;          /* no non‑trivial divisor */

    uint64_t m = (uint64_t)rev_n;      /* work in native uint64_t */

    /* store distinct prime factors and their exponents */
    uint64_t primes[64];
    int       exps[64];
    int       cnt = 0;

    uint64_t temp = m;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]  = e;
            ++cnt;
        }
    }
    if (temp > 1) {                     /* last prime factor */
        primes[cnt] = temp;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* find the minimum semiprime divisor */
    unsigned __int128 best = 0;         /* 0 means none found yet */

    /* squares of primes with exponent >= 2 */
    for (int i = 0; i < cnt; ++i) {
        if (exps[i] >= 2) {
            unsigned __int128 cand = (unsigned __int128)primes[i] * primes[i];
            if (best == 0 || cand < best) best = cand;
        }
    }

    /* products of two distinct primes */
    for (int i = 0; i < cnt; ++i) {
        for (int j = i + 1; j < cnt; ++j) {
            unsigned __int128 cand = (unsigned __int128)primes[i] * primes[j];
            if (best == 0 || cand < best) best = cand;
        }
    }

    return (best == 0) ? (seq_t)0 : (seq_t)best;
}
