/*
 * A238528  Record prime values of A238525.
 * https://oeis.org/A238528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime among the record values of A238525 (A238525(k)
 *            = k mod sopfr(k)); computed by a segmented scan with memoization
 * Range:     u8 n <= 19, u16 n <= 862, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 8723 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t A238525_a(idx_t n)
{
    if (n < 2) return 0;
    idx_t m = n;
    seq_t sum = 0;
    while ((m & 1ULL) == 0) { sum += 2; m >>= 1; }
    for (idx_t p = 3; p <= m / p; p += 2)
        while (m % p == 0) { sum += (seq_t)p; m /= p; }
    if (m > 1) sum += (seq_t)m;
    return ((seq_t)n) % sum;
}

static bool A238528_isprime(uint64_t v)
{
    if (v < 2) return false;
    if (v % 2 == 0) return v == 2;
    for (uint64_t d = 3; d <= v / d; d += 2)
        if (v % d == 0) return false;
    return true;
}

static uint64_t *A238528_pr = 0;
static size_t A238528_pcnt = 0;
static uint64_t A238528_plimit = 0;

static void A238528_primes_upto(uint64_t need)
{
    if (need <= A238528_plimit) return;
    uint64_t lim = 2 * need + 1000;
    char *comp = (char *)calloc((size_t)lim + 1, 1);
    free(A238528_pr);
    A238528_pr = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)lim / 2 + 10));
    A238528_pcnt = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (!comp[i]) {
            A238528_pr[A238528_pcnt++] = i;
            for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
        }
    }
    free(comp);
    A238528_plimit = lim;
}

#define A238528_B 65536

static uint64_t *A238528_res = 0;
static size_t A238528_cnt = 0, A238528_cap = 0;
static uint64_t A238528_next = 2;
static uint64_t A238528_max = 0;

seq_t a(idx_t n)
{
    static uint64_t rem[A238528_B], sm[A238528_B];
    while (A238528_cnt < (size_t)n) {
        uint64_t lo = A238528_next, hi = lo + A238528_B;
        uint64_t sq = 1;
        while (sq * sq < hi) sq++;
        A238528_primes_upto(sq + 1);
        for (uint64_t i = 0; i < A238528_B; i++) { rem[i] = lo + i; sm[i] = 0; }
        for (size_t k = 0; k < A238528_pcnt; k++) {
            uint64_t p = A238528_pr[k];
            if (p * p >= hi) break;
            uint64_t s = ((lo + p - 1) / p) * p;
            for (uint64_t x = s; x < hi; x += p) {
                uint64_t i = x - lo;
                while (rem[i] % p == 0) { rem[i] /= p; sm[i] += p; }
            }
        }
        for (uint64_t i = 0; i < A238528_B; i++) {
            uint64_t sum = sm[i] + (rem[i] > 1 ? rem[i] : 0);
            uint64_t v = (lo + i) % sum;
            if (v > A238528_max) {
                A238528_max = v;
                if (A238528_isprime(v)) {
                    if (A238528_cnt == A238528_cap) {
                        A238528_cap = A238528_cap ? A238528_cap * 2 : 64;
                        A238528_res = (uint64_t *)realloc(A238528_res, sizeof(uint64_t) * A238528_cap);
                    }
                    A238528_res[A238528_cnt++] = v;
                }
            }
        }
        A238528_next = hi;
    }
    return (seq_t)A238528_res[n - 1];
}
