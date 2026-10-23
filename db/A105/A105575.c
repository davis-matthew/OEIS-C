/*
 * A105575  Largest primes < numbers of the form 6k (duplicates removed).
 * https://oeis.org/A105575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n‑th distinct largest prime p such that
 *            p < 6k for some k, where k is iterated from 1 upward.
 *            Duplicates are removed: if the same prime occurs again
 *            for a larger k it is skipped.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

/* ---------- small helper functions ---------- */

static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* deterministic Miller–Rabin for 64‑bit integers */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes first */
    static const uint64_t small[12] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,
                                       17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (int i = 0; i < 12; ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }

    /* write n-1 as d * 2^s */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    /* bases that guarantee correctness up to 2^64 */
    static const uint64_t witnesses[7] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };

    for (int i = 0; i < 7; ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    seq_t last = 0;        /* last distinct prime found */
    idx_t  count = 0;      /* how many distinct primes found */
    idx_t  k = 1;          /* current multiple of 6 */

    while (count < n) {
        /* 6 * k */
        uint64_t m = 6ULL * k;
        /* start from 6k - 1 (odd) and go downward by 2 */
        uint64_t cand = m - 1;
        while (!isPrime64(cand)) {
            cand -= 2;
        }

        if (cand != last) {
            last = (seq_t)cand;
            ++count;
        }
        ++k;
    }
    return last;
}
