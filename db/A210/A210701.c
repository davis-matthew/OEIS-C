/*
 * A210701  The location of records in A210700.
 * https://oeis.org/A210701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   no closed form; segmented-sieve search: a(n) = n-th index k at
 *            which A210700(k) = min{m >= 0 : 3k-m and 3k+m both prime} sets a
 *            new record
 * Range:     u8 n <= 14, u16 n <= 33, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 53 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"

#define A210701_MARGIN 20000ULL
#define A210701_HALF   (1ULL << 22)          /* odd numbers per segment */
#define A210701_W      (2ULL * A210701_HALF) /* integers per segment */
#define A210701_BLIM   (1u << 20)            /* base primes limit */

static uint32_t *A210701_bp = NULL;
static size_t A210701_nbp = 0;
static uint8_t *A210701_flags = NULL;
static uint64_t A210701_segLo = 0, A210701_segHi = 0;
static bool A210701_segValid = false;

static bool A210701_pr(uint64_t x)
{
    static const unsigned sp[] = {5, 7, 11, 13, 17, 19, 23};
    if (x < 2) return false;
    if (x < 4) return true;
    if ((x & 1) == 0 || x % 3 == 0) return false;
    for (size_t i = 0; i < sizeof(sp) / sizeof(sp[0]); i++) {
        if (x == sp[i]) return true;
        if (x % sp[i] == 0) return false;
    }
    return is_prime_u64(x);
}

static void A210701_init_base(void)
{
    if (A210701_bp) return;
    uint8_t *s = (uint8_t *)malloc(A210701_BLIM + 1);
    memset(s, 1, A210701_BLIM + 1);
    s[0] = s[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= A210701_BLIM; i++)
        if (s[i])
            for (uint32_t j = i * i; j <= A210701_BLIM; j += i) s[j] = 0;
    size_t c = 0;
    for (uint32_t i = 3; i <= A210701_BLIM; i++) if (s[i]) c++;
    A210701_bp = (uint32_t *)malloc(c * sizeof(uint32_t));
    c = 0;
    for (uint32_t i = 3; i <= A210701_BLIM; i++) if (s[i]) A210701_bp[c++] = i;
    A210701_nbp = c;
    free(s);
    A210701_flags = (uint8_t *)malloc(A210701_HALF);
}

static void A210701_build(uint64_t c)
{
    uint64_t lo = (c > A210701_MARGIN) ? ((c - A210701_MARGIN) & ~1ULL) : 0;
    uint64_t hi = lo + A210701_W;
    A210701_segLo = lo;
    A210701_segHi = hi;
    if ((uint64_t)A210701_BLIM * A210701_BLIM < hi) { /* sieve can't cover */
        A210701_segValid = false;
        return;
    }
    uint8_t *f = A210701_flags;
    memset(f, 1, A210701_HALF);
    if (lo == 0) f[0] = 0; /* x = 1 */
    for (size_t k = 0; k < A210701_nbp; k++) {
        uint64_t p = A210701_bp[k];
        if (p * p >= hi) break;
        uint64_t st = p * p;
        if (st < lo) st = ((lo + p - 1) / p) * p;
        if ((st & 1) == 0) st += p;
        uint64_t step = 2 * p;
        for (uint64_t j = st; j < hi; j += step) f[(j - lo - 1) >> 1] = 0;
    }
    A210701_segValid = true;
}

static inline bool A210701_isp(uint64_t x)
{
    if (A210701_segValid && x >= A210701_segLo && x < A210701_segHi) {
        if ((x & 1) == 0) return x == 2;
        return A210701_flags[(x - A210701_segLo - 1) >> 1] != 0;
    }
    return A210701_pr(x);
}

static uint64_t A210701_minm(uint64_t n)
{
    if (n == 1) return 0;
    uint64_t c = 3 * n;
    uint64_t m = (n & 1) ? 2 : 1;
    for (;; m += 2) {
        if (m % 3 == 0) continue;
        if (m >= c) return m;
        if (A210701_isp(c - m) && A210701_isp(c + m)) return m;
    }
}

seq_t a(idx_t n)
{
    static uint64_t recs[512];
    static size_t cnt = 0;
    static uint64_t next = 1;
    static bool have = false;
    static uint64_t best = 0;

    A210701_init_base();
    while (cnt < (size_t)n && cnt < 512) {
        uint64_t c = 3 * next;
        if (next > 1 && (A210701_segHi == 0 || c + A210701_MARGIN >= A210701_segHi))
            A210701_build(c);
        uint64_t m = A210701_minm(next);
        if (!have || m > best) {
            have = true;
            best = m;
            recs[cnt++] = next;
        }
        next++;
    }
    return (seq_t)recs[n - 1];
}
