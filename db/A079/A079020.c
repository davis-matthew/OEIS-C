/*
 * A079020  Suppose p and q = p+20 are primes. Define the difference pattern
 *          of (p,q) to be the successive differences of the primes in the
 *          range p to q. There are 56 possible difference patterns, shown in
 *          the Comments line. Sequence gives smallest value of p for each
 *          difference pattern, sorted by magnitude.
 * https://oeis.org/A079020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   Computation of OEIS A079020 – smallest prime p for each 20‑gap
 *            difference pattern (p and p+20 are both prime), sorted by p
 * Range:     u8 n <= 15, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

/* Helper structure to store a discovered pattern */
typedef struct {
    int len;                     /* number of differences */
    uint8_t diffs[12];           /* differences (always <=20) */
    uint64_t p;                  /* smallest prime yielding this pattern */
} pattern_t;

/* maximum expected patterns */
#define MAX_PATTERNS 60
/* maximum number of primes inside a 20‑gap */
#define MAX_DIFFS 12

/* Compare two uint64_t values for qsort. */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* ===================== main sequence function ===================== */

seq_t a(idx_t n)
{
    /* static storage for computed results */
    static seq_t result[56];
    static bool initialized = false;

    if (!initialized) {
        /* ---------------- Search phase ---------------- */
        pattern_t patterns[MAX_PATTERNS];
        int patterns_found = 0;

        uint64_t p = 1;                     /* start before first prime */
        while (patterns_found < 56) {        /* there are exactly 56 patterns */
            p = next_prime(p);              /* next prime after current p */

            uint64_t q = p + 20;
            if (!is_prime(q)) continue;     /* need q to also be prime */

            /* gather primes in [p, q] and compute differences */
            uint8_t diffs[MAX_DIFFS];
            int len = 0;
            uint64_t cur = p;
            uint64_t prev = 0;
            while (cur <= q) {
                if (prev != 0) {
                    diffs[len++] = (uint8_t)(cur - prev);
                }
                prev = cur;
                cur = next_prime(cur);
            }

            /* compare with existing patterns */
            bool new_pattern = true;
            for (int i = 0; i < patterns_found; ++i) {
                if (patterns[i].len != len) continue;
                bool same = true;
                for (int k = 0; k < len; ++k) {
                    if (patterns[i].diffs[k] != diffs[k]) {
                        same = false;
                        break;
                    }
                }
                if (same) {
                    if (p < patterns[i].p) patterns[i].p = p;
                    new_pattern = false;
                    break;
                }
            }
            if (new_pattern) {
                patterns[patterns_found].len = len;
                for (int k = 0; k < len; ++k) patterns[patterns_found].diffs[k] = diffs[k];
                patterns[patterns_found].p = p;
                ++patterns_found;
            }
        }

        /* ---------------- Sorting phase ---------------- */
        uint64_t tmp[56];
        for (int i = 0; i < 56; ++i) tmp[i] = patterns[i].p;
        qsort(tmp, 56, sizeof(uint64_t), cmp_uint64);

        /* store sorted results in seq_t type */
        for (int i = 0; i < 56; ++i) result[i] = (seq_t)tmp[i];

        initialized = true;
    }

    /* return requested term (1‑based) */
    if (n == 0 || n > 56) return 0;
    return result[(idx_t)n - 1];
}
