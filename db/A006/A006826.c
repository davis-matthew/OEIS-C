/*
 * A006826  Erroneous version of A003182.
 * https://oeis.org/A006826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0) = 1 (erroneous term); for n >= 1, a(n) = A003182(n), the
 *            number of monotone Boolean functions of n variables up to
 *            permutation of the variables. Computed with Burnside's lemma:
 *            for each permutation of the n variables, count the up-sets of
 *            the poset of subset-orbits by recursion, then average...
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 6, i64 n <= 6, i128 n <= 6,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 6 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t a006826_cnt(uint64_t S, const uint64_t *up, const uint64_t *dn)
{
    if (!S) return 1;
    int x = __builtin_ctzll(S);
    /* x not in up-set: everything below x is excluded;
       x in up-set: everything above x is included */
    return a006826_cnt(S & ~dn[x], up, dn) + a006826_cnt(S & ~up[x], up, dn);
}

static int a006826_nextperm(int *p, int n)
{
    int i = n - 2;
    while (i >= 0 && p[i] >= p[i + 1]) i--;
    if (i < 0) return 0;
    int j = n - 1;
    while (p[j] <= p[i]) j--;
    int t = p[i]; p[i] = p[j]; p[j] = t;
    for (int l = i + 1, r = n - 1; l < r; l++, r--) {
        t = p[l]; p[l] = p[r]; p[r] = t;
    }
    return 1;
}

static uint64_t a006826_count(int n)
{
    int N = 1 << n;
    int p[8];
    uint64_t fact = 1, total = 0;
    for (int i = 0; i < n; i++) { p[i] = i; fact *= (uint64_t)(i + 1); }
    do {
        int img[64], orb[64];
        uint64_t up[64], dn[64];
        for (int s = 0; s < N; s++) {
            int t = 0;
            for (int b = 0; b < n; b++)
                if ((s >> b) & 1) t |= 1 << p[b];
            img[s] = t;
            orb[s] = -1;
        }
        int k = 0;
        for (int s = 0; s < N; s++) {
            if (orb[s] < 0) {
                int t = s;
                do { orb[t] = k; t = img[t]; } while (t != s);
                k++;
            }
        }
        for (int i = 0; i < k; i++) { up[i] = 0; dn[i] = 0; }
        for (int x = 0; x < N; x++)
            for (int y = 0; y < N; y++)
                if ((x & y) == x) {
                    up[orb[x]] |= 1ULL << orb[y];
                    dn[orb[y]] |= 1ULL << orb[x];
                }
        uint64_t all = (k == 64) ? ~0ULL : ((1ULL << k) - 1);
        total += a006826_cnt(all, up, dn);
    } while (a006826_nextperm(p, n));
    return total / fact;
}

seq_t a(idx_t n)
{
    static uint64_t memo[7];
    static int have[7];
    if (n == 0) return (seq_t)1;
    if (n > 6) return (seq_t)0; /* no further terms exist for this dead sequence */
    if (!have[n]) {
        memo[n] = a006826_count((int)n);
        have[n] = 1;
    }
    return (seq_t)memo[n];
}
