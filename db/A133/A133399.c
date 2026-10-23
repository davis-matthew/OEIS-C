/*
 * A133399  Triangle T(n,k) = number of forests of labeled rooted trees with n
 *          nodes, containing exactly k trees of height one, all others having
 *          height zero (n>=0, 0<=k<=floor(n/2)).
 * https://oeis.org/A133399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 59, u64 n <= 150,
 *            u128 n <= 409, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the flattened OEIS triangle T(n,k):
 *  T(n,k) = C(n,k) * k! * S(n-k+1, k+1)   (0 ≤ k ≤ ⌊n/2⌋)
 *  The sequence lists the rows of the triangle in order:
 *  row 0, row 1, … , each row is k = 0…⌊n/2⌋.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/binom.h"

#define MAX_M 500          /* enough for n up to 10000 */
#define MAX_N 520          /* maximum n in Stirling recurrence */
#define MAX_K 260          /* maximum k in Stirling recurrence */

/* pre‑computed Stirling numbers of the second kind S(n,k) */
static seq_t S[MAX_N+1][MAX_K+1];
/* factorial table for k up to MAX_K */
static seq_t fact[MAX_K+1];
/* prefix array: prefix[i] = total terms in rows 0..i-1 */
static idx_t prefix[MAX_M+2];
/* flag indicating whether tables were initialized */
static bool init_done = false;

/* one‑time initialization of tables */
static void init_tables(void)
{
    if (init_done) return;
    init_done = true;

    /* Stirling numbers: S(0,0)=1, S(n,0)=0 for n>0, recurrence
     * S(n,k) = k*S(n-1,k) + S(n-1,k-1)   (1 ≤ k ≤ n)
     */
    for (idx_t i = 0; i <= MAX_N; ++i) {
        for (idx_t j = 0; j <= MAX_K; ++j)
            S[i][j] = (seq_t)0;
    }
    S[0][0] = (seq_t)1;
    for (idx_t n = 1; n <= MAX_N; ++n) {
        for (idx_t k = 1; k <= MAX_K && k <= n; ++k) {
            seq_t term1 = (seq_t)k * S[n-1][k];
            seq_t term2 = S[n-1][k-1];
            S[n][k] = term1 + term2;
        }
    }

    /* factorial table */
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= MAX_K; ++i)
        fact[i] = fact[i-1] * (seq_t)i;

    /* prefix array: number of entries in each row */
    prefix[0] = (idx_t)0;
    for (idx_t m = 0; m <= MAX_M; ++m) {
        idx_t terms;
        if (m < 4) {
            terms = (m <= 1) ? 1 : 2;   /* rows 0,1 have 1 term; 2,3 have 2 terms */
        } else {
            terms = (m / 2) + 1;        /* floor(m/2)+1 terms for m>=4 */
        }
        prefix[m+1] = prefix[m] + terms;
    }
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    init_tables();

    /* binary search to find row m such that prefix[m] ≤ n < prefix[m+1] */
    idx_t low = 0, high = MAX_M;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (prefix[mid+1] <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t m = low;
    idx_t offset = n - prefix[m];
    idx_t k = offset;                 /* k runs from 0 to floor(m/2) in ascending order */

    /* Compute T(m,k) = C(m,k) * k! * S(m-k+1, k+1) */
    seq_t bin = binom((idx_t)m, (idx_t)k);          /* C(m,k) */
    seq_t val = bin * fact[(idx_t)k];               /* multiply by k! */
    seq_t stir = S[(idx_t)(m - k + 1)][(idx_t)(k + 1)];
    val *= stir;
    return val;
}
