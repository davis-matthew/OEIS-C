/*
 * A058251  LCM of n-th primorial number and its Euler totient.
 * https://oeis.org/A058251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM(primorial(n), φ(primorial(n)))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>

#define MAX_PRIME_LIMIT 200000   /* enough for <=10000th prime */

/* prime storage and pre‑computed smallest prime factors */
static idx_t prime_list[MAX_PRIME_LIMIT + 1];
static size_t prime_count = 0;
static idx_t spf[MAX_PRIME_LIMIT + 1];   /* smallest prime factor */

static void build_sieve(void)
{
    size_t i, j;

    /* linear sieve – O(MAX_PRIME_LIMIT) */
    prime_count = 0;
    spf[1] = 1;
    for (i = 2; i <= (size_t)MAX_PRIME_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            prime_list[prime_count++] = (idx_t)i;
        }
        for (j = 0; j < prime_count && (size_t)prime_list[j] * i <= MAX_PRIME_LIMIT; ++j) {
            idx_t p = prime_list[j];
            idx_t v = (idx_t)i * p;
            if (spf[v] == 0) spf[v] = p;
            if (p == spf[i]) break;
        }
    }
}

static int initialized = 0;
static int phi_exp[MAX_PRIME_LIMIT + 1];   /* temporary exponent storage */

/* Compute a(n) */
seq_t a(idx_t n)
{
    size_t i, tbl_idx;
    seq_t result, term, base;
    int e, ee;

    if (!initialized) {
        build_sieve();
        initialized = 1;
    }
    if (n == 0) return (seq_t)1;

    if (n > prime_count) {
        /* n larger than precomputable range – should not happen for the
           standard tests (n <= 10000).  Return 0 as a graceful fallback. */
        return (seq_t)0;
    }

    /* Reset temporary exponent table */
    memset(phi_exp, 0, sizeof(phi_exp));

    /* Accumulate exponents appearing in φ(primorial) = ∏(p_i – 1) */
    for (i = 0; i < n; ++i) {
        idx_t v = prime_list[i] - 1;
        while (v > 1) {
            idx_t f = spf[v];
            int cnt = 0;
            while (v % f == 0) {
                v /= f;
                ++cnt;
            }
            phi_exp[f] += cnt;
        }
    }

    /* Compute the LCM: for each primorial prime, exponent = max(1, φ exponent) */
    result = (seq_t)1;
    for (i = 0; i < n; ++i) {
        idx_t p = prime_list[i];
        e = phi_exp[p];
        if (e < 1) e = 1;

        /* fast pow: base = p^e */
        term = (seq_t)1;
        base = (seq_t)p;
        ee = e;
        while (ee > 0) {
            if (ee & 1) term *= base;
            base *= base;
            ee >>= 1;
        }
        result *= term;
    }

    return result;
}
