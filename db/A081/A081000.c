/*
 * A081000  n is a member if and only if it ranks among top n positive
 *          integers in centrality (cf. A080997 for fuller description of this
 *          concept).
 * https://oeis.org/A081000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with #{m : A018804(m)/m^2 > A018804(k)/k^2} <
 *            k (search; A018804 via multiplicative sieve)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *A081000_P = 0;
static uint64_t A081000_lim = 0;

/* Pillai function P(m) for m <= L, via multiplicative sieve */
static void A081000_ensure(uint64_t L)
{
    if (L <= A081000_lim) return;
    if (L < 2 * A081000_lim) L = 2 * A081000_lim;
    if (L < 1024) L = 1024;
    uint32_t *spf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint64_t *P = (uint64_t *)malloc((size_t)(L + 1) * sizeof(uint64_t));
    for (uint64_t i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (uint64_t j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    P[0] = 0;
    P[1] = 1;
    for (uint64_t m = 2; m <= L; m++) {
        uint64_t p = spf[m], r = m, pe = 1, e = 0;
        while (r % p == 0) {
            r /= p;
            e++;
            if (r % p == 0) pe *= p;
        }
        P[m] = P[r] * (pe * ((e + 1) * p - e));
    }
    free(spf);
    free(A081000_P);
    A081000_P = P;
    A081000_lim = L;
}

static bool A081000_is_member(uint64_t k)
{
    A081000_ensure(k);
    unsigned __int128 Pk = A081000_P[k];
    unsigned __int128 k6 = (unsigned __int128)k * k * k * k * k * k;
    uint64_t lo = 1, hi = k * k + 10;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        unsigned __int128 lhs = (unsigned __int128)mid * mid * Pk * Pk * Pk;
        if (lhs >= 4 * k6) hi = mid; else lo = mid + 1;
    }
    uint64_t bound = lo;
    A081000_ensure(bound > k ? bound : k);
    uint64_t count = 0;
    unsigned __int128 k2 = (unsigned __int128)k * k;
    for (uint64_t m = 1; m <= bound; m++) {
        if (m == k) continue;
        unsigned __int128 lhs = (unsigned __int128)A081000_P[m] * k2;
        unsigned __int128 rhs = Pk * ((unsigned __int128)m * m);
        if (lhs > rhs) {
            count++;
            if (count >= k) return false;
        }
    }
    return true;
}

static uint64_t *A081000_terms = 0;
static uint64_t A081000_nterms = 0, A081000_cap = 0, A081000_next = 1;

seq_t a(idx_t n)
{
    while (A081000_nterms < (uint64_t)n) {
        uint64_t k = A081000_next++;
        if (A081000_is_member(k)) {
            if (A081000_nterms == A081000_cap) {
                A081000_cap = A081000_cap ? A081000_cap * 2 : 64;
                A081000_terms = (uint64_t *)realloc(A081000_terms, A081000_cap * sizeof(uint64_t));
            }
            A081000_terms[A081000_nterms++] = k;
        }
    }
    return (seq_t)A081000_terms[n - 1];
}
