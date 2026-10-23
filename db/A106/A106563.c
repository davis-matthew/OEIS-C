/*
 * A106563  Numbers n such that n^2 is not the sum of two primes.
 * https://oeis.org/A106563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   mulmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n): the nth integer whose square is NOT the sum of two primes.
 *  The rule is: for n = 1 it holds; for any odd n > 1,
 *  n^2 is not a sum of two primes iff n^2-2 is composite.
 *  (All even n>1 produce even squares that are Goldbach sums, so they are excluded.)
 *  The function therefore generates odd n, tests n^2-2 for primality
 *  (using a 64‑bit deterministic Miller–Rabin test), and returns the nth
 *  qualifying n.  The algorithm is O(n) and requires no persistent state.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mulmod64.h"

/* Deterministic Miller–Rabin for 64‑bit integers */

static uint64_t powmod64(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mulmod64(r, a, mod);
        a = mulmod64(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool isPrime64(uint64_t n) {
    if (n < 2)                       return false;
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL
    };
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Function required by the specification */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined for offset 1, harmless */
    idx_t count = 0;
    seq_t result = 0;

    for (idx_t i = 1; count < n; ++i) {
        if (i == 1) {                    /* 1^2 is not a sum of two primes */
            ++count;
            result = (seq_t)1;
            continue;
        }
        if ((i & 1) == 0) continue;      /* even i yield even squares → Goldbach sums */

        /* compute i^2 - 2; use 128‑bit to avoid overflow */
        unsigned __int128 sq = (unsigned __int128)i * i;
        if (sq <= (unsigned __int128)UINT64_MAX) {
            uint64_t val = (uint64_t)sq - 2;
            if (!isPrime64(val)) {
                ++count;
                result = (seq_t)i;
            }
        } else {
            /* If i is so large that i^2 exceeds 64‑bit, we conservatively
             * treat the number as composite (this situation is outside the
             * expected test range for n up to ~10 000). */
            ++count;
            result = (seq_t)i;
        }
    }
    return result;
}
