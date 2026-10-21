/*
 * A090181  Triangle of Narayana (A001263) with 0 <= k <= n, read by rows.
 * https://oeis.org/A090181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the flattened triangle of Narayana numbers. T(r,k) = 1
 *            if r = 0, k = 0 T(r,k) = 0 if r > 0, k = 0 T(r,k) =
 *            binomial(r,k) * binomial(r,k-1) / r (r>0, 1 ≤ k ≤ r) a(n) =
 *            T(r,k) where n = r(r+1)/2 + k (rows are 0‑based)
 * Range:     u8 n <= 39, u16 n <= 95, u32 n <= 240, u64 n <= 758,
 *            u128 n <= 2588, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- prime factorisation helpers ---------- */
static idx_t *spf = NULL;      /* smallest prime factor array      */
static idx_t spf_cap = 0;      /* maximum value for which spf is valid */

/* Ensure that spf is valid for all numbers up to limit */
static void ensure_spf_upto(idx_t limit)
{
    if (limit <= spf_cap) return;
    free(spf);
    spf_cap = limit;
    spf = (idx_t *)malloc((spf_cap + 1) * sizeof(idx_t));
    if (!spf) exit(1);
    for (idx_t i = 0; i <= spf_cap; ++i) spf[i] = 0;
    if (spf_cap >= 1) spf[1] = 1;
    for (idx_t i = 2; i * i <= spf_cap; ++i)
        if (spf[i] == 0) {
            spf[i] = i;
            for (idx_t j = i * i; j <= spf_cap; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    for (idx_t i = 2; i <= spf_cap; ++i) if (spf[i] == 0) spf[i] = i;
}

/* Add or subtract prime exponents for the factorisation of num.
   delta = +1 for adding, -1 for subtracting.  exps is a
   static array of int large enough for indices up to spf_cap. */
static void factor_add(idx_t num, int delta, int *exps)
{
    while (num > 1) {
        idx_t p = spf[num];
        int cnt = 0;
        do {
            num /= p;
            ++cnt;
        } while (num % p == 0);
        exps[p] += delta * cnt;
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Find the row r and column k such that a(n) = T(r,k). */
    idx_t r = 0, start = 0;
    while (start + r + 1 <= n) {
        start += r + 1;
        ++r;
    }
    idx_t k = n - start;

    /* Special cases */
    if (r == 0) return (seq_t)1;
    if (k == 0) return (seq_t)0;

    /* Prepare prime factor exponent array */
    ensure_spf_upto(r);
    /* Static exponent array reused for each call */
    static int *exps = NULL;
    static size_t exps_cap = 0;
    if (r + 1 > exps_cap) {
        free(exps);
        exps_cap = r + 1;
        exps = (int *)calloc(exps_cap, sizeof(int));
    } else if (exps_cap > 0) {
        memset(exps, 0, (r + 1) * sizeof(int));
    } else {
        exps = (int *)calloc(exps_cap, sizeof(int));
    }

    /* Count exponents for binomial(r, k) */
    for (idx_t i = r - k + 1; i <= r; ++i) factor_add(i, +1, exps);
    for (idx_t j = 1; j <= k; ++j) factor_add(j, -1, exps);

    /* Count exponents for binomial(r, k-1) */
    for (idx_t i = r - k + 2; i <= r; ++i) factor_add(i, +1, exps);
    for (idx_t j = 1; j <= k - 1; ++j) factor_add(j, -1, exps);

    /* Subtract the factor r (division by r) */
    factor_add(r, -1, exps);

    /* Multiply the remaining prime powers to obtain the result. */
    seq_t result = 1;
    for (idx_t p = 2; p <= r; ++p) {
        int e = exps[p];
        for (int t = 0; t < e; ++t) result *= (seq_t)p;
    }
    return result;
}
