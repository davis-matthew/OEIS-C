/*
 * A136205  Square array read by antidiagonals: T(m,n) =
 *          H(n,2*m)*(2*m)!/(2*m+2*n-1). H(0,m) = 1/m, for all positive
 *          integers m. H(n,m) = Sum_{k=1..m} H(n-1,k).
 * https://oeis.org/A136205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,narray) read by antidiagonals, where (m,narray) are
 *            the coordinates obtained from n. The value is computed by
 *            H(narray,2m)*(2m)!/(2m+2*narray-1) where H(0,m) = 1/m and H(k,m)
 *            = sum_{i=1..m} H(k-1,i)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 18, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   factorial, gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/factorial.h"
#include "../utils/gcd_seq.h"

typedef struct { seq_t num; seq_t den; } rat_t;

/* Compute gcd for seq_t type */

/* Normalize a rational number (num/den) to lowest terms */
static inline void rat_set(rat_t *r, seq_t num, seq_t den)
{
    if (den == 0) { r->num = 0; r->den = 0; return; }
    seq_t g = gcd_seq(num, den);
    r->num = num / g;
    r->den = den / g;
}

/* r = a + b  (a and b are rationals) */
static inline void rat_add(rat_t *r, const rat_t *a, const rat_t *b)
{
    seq_t num = a->num * b->den + b->num * a->den;
    seq_t den = a->den * b->den;
    rat_set(r, num, den);
}

/* Maximum value of 2*m for n <= 10000 is < 300 (since d ≈ 141) */
#define MAX_TWO_M 300

seq_t a(idx_t n)
{
    /* Compute antidiagonal coordinates */
    idx_t d = (idx_t)((sqrt((double)(8*n + 1)) - 1.0) / 2.0);
    idx_t tri = d * (d + 1) / 2;
    idx_t k = n - tri;              /* position on diagonal */
    idx_t m = k + 1;                /* m >= 1 */
    idx_t narray = d - k;           /* narray >= 0 */

    idx_t M = 2 * m;                /* size needed for H */
    if (M > MAX_TWO_M) {            /* safety; should not happen for n <= 10000 */
        M = MAX_TWO_M;
    }

    /* Prepare two arrays for dynamic programming of H */
    static rat_t Hprev[MAX_TWO_M + 1];
    static rat_t Hcurr[MAX_TWO_M + 1];

    /* Initialize H(0,i) = 1/i for i = 1..M */
    for (idx_t i = 1; i <= M; ++i) {
        rat_set(&Hprev[i], (seq_t)1, (seq_t)i);
    }

    /* Iterate narray levels to compute H(narray, M) */
    for (idx_t level = 1; level <= narray; ++level) {
        rat_set(&Hcurr[1], Hprev[1].num, Hprev[1].den);
        for (idx_t i = 2; i <= M; ++i) {
            rat_add(&Hcurr[i], &Hcurr[i-1], &Hprev[i]);
        }
        /* Copy Hcurr into Hprev for next level */
        for (idx_t i = 1; i <= M; ++i) {
            Hprev[i].num = Hcurr[i].num;
            Hprev[i].den = Hcurr[i].den;
        }
    }

    /* Retrieve H(narray, M) */
    seq_t hnum = Hprev[M].num;
    seq_t hden = Hprev[M].den;

    /* Compute factorial(2*m) */
    seq_t fact2m = factorial(M);

    /* Compute the final value:
       T = (hnum / hden) * fact2m / (2*m + 2*narray - 1) */
    idx_t denom_factor = 2 * m + 2 * narray - 1;
    seq_t numerator   = hnum * fact2m;
    seq_t denominator = hden * (seq_t)denom_factor;
    seq_t result = numerator / denominator;  /* exact division */

    return result;
}
