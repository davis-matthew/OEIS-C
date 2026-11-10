/*
 * A071855  Numbers n such that A067434(n) = A000720(n) + A000005(n).
 * https://oeis.org/A071855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A067434(k) = A000720(k) + A000005(k);
 *            found by incremental search with a prime sieve
 * Range:     u8 n <= 21, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *A071855_terms = NULL;
static size_t A071855_cnt = 0, A071855_cap = 0;
static uint64_t A071855_next = 1;   /* next k to test */
static uint64_t A071855_pi = 0;     /* pi(next-1) */

static uint64_t *A071855_pr = NULL;
static size_t A071855_npr = 0;
static uint64_t A071855_lim = 0;
static unsigned char *A071855_sv = NULL;

static void A071855_sieve(uint64_t L)
{
    free(A071855_sv);
    free(A071855_pr);
    A071855_sv = (unsigned char *)calloc((size_t)L + 1, 1);
    A071855_pr = (uint64_t *)malloc(((size_t)L / 2 + 10) * sizeof(uint64_t));
    A071855_npr = 0;
    for (uint64_t i = 2; i <= L; i++) {
        if (!A071855_sv[i]) {
            A071855_pr[A071855_npr++] = i;
            for (uint64_t j = i * i; j <= L; j += i) A071855_sv[j] = 1;
        }
    }
    A071855_lim = L;
}

static uint64_t A071855_ndiv(uint64_t m)
{
    uint64_t r = 1;
    for (uint64_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            r *= (e + 1);
        }
    }
    if (m > 1) r *= 2;
    return r;
}

seq_t a(idx_t n)
{
    while (A071855_cnt < (size_t)n) {
        uint64_t k = A071855_next;
        if (2 * k > A071855_lim)
            A071855_sieve(A071855_lim ? 2 * A071855_lim + 2 * k : 4096 + 2 * k);
        if (k >= 2 && !A071855_sv[k]) A071855_pi++;
        uint64_t two = 2 * k, cnt = 0;
        for (size_t i = 0; i < A071855_npr; i++) {
            uint64_t p = A071855_pr[i];
            if (p > two) break;
            if (p > k) { cnt++; continue; }
            uint64_t pw = p;
            int ok = 0;
            for (;;) {
                if ((two / pw) - 2 * (k / pw)) { ok = 1; break; }
                if (pw > two / p) break;
                pw *= p;
            }
            if (ok) cnt++;
        }
        if (cnt == A071855_pi + A071855_ndiv(k)) {
            if (A071855_cnt == A071855_cap) {
                A071855_cap = A071855_cap ? A071855_cap * 2 : 64;
                A071855_terms = (uint64_t *)realloc(A071855_terms, A071855_cap * sizeof(uint64_t));
            }
            A071855_terms[A071855_cnt++] = k;
        }
        A071855_next++;
    }
    return (seq_t)A071855_terms[(size_t)n - 1];
}
