/*
 * A164297  Let S(n) be the set of all positive integers that are <= n and are
 *          coprime to n. a(n) = the number of members of S(n) that are each
 *          non-coprime with at least one other member of S(n).
 * https://oeis.org/A164297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of elements in the reduced residue system S(n)
 *            that are not coprime with at least one other element of S(n)
 * Range:     u8 n <= 292, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

/* Compute gcd for idx_t (uint64_t). */
static inline idx_t ugcd(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Global smallest prime factor sieve. */
static idx_t *global_spf = NULL;
static idx_t global_spf_size = 0;

static void ensure_spf(idx_t n) {
    if (n >= global_spf_size) {
        if (global_spf) free(global_spf);
        build_spf(n, &global_spf);
        global_spf_size = n + 1;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* defined only for n>=1, but guard anyway */

    /* Ensure spf up to n */
    ensure_spf(n);
    idx_t *spf = global_spf;

    /* Arrays for prime division by n and prime counts */
    bool *pdiv = (bool*)malloc((n + 1) * sizeof(bool));
    int  *cnt  = (int *)malloc((n + 1) * sizeof(int));
    if (!pdiv || !cnt) {
        free(pdiv); free(cnt);
        return (seq_t)0;   /* fallback on allocation failure */
    }
    memset(pdiv, 0, (n + 1) * sizeof(bool));
    memset(cnt,  0, (n + 1) * sizeof(int));

    /* Mark primes dividing n */
    idx_t temp = n;
    while (temp > 1) {
        idx_t p = spf[temp];
        pdiv[p] = true;
        while (temp % p == 0) temp /= p;
    }

    /* First pass: count occurrences of each prime (not dividing n) among S(n) */
    for (idx_t a = 1; a <= n; ++a) {
        if (ugcd(a, n) != 1) continue;   /* not coprime */

        idx_t x = a;
        while (x > 1) {
            idx_t p = spf[x];
            while (x % p == 0) x /= p;
            if (!pdiv[p]) ++cnt[p];
        }
    }

    /* Second pass: count numbers that share such a prime with another */
    idx_t ans = 0;
    for (idx_t a = 1; a <= n; ++a) {
        if (ugcd(a, n) != 1) continue;   /* not coprime */

        idx_t x = a;
        bool good = false;
        while (x > 1) {
            idx_t p = spf[x];
            while (x % p == 0) x /= p;
            if (!pdiv[p] && cnt[p] >= 2) {
                good = true;
                break;
            }
        }
        if (good) ++ans;
    }

    free(pdiv);
    free(cnt);

    return (seq_t)ans;
}
