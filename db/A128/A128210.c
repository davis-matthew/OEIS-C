/*
 * A128210  Number triangle T(n,k) = (-1)^(n-k)*[k<=n]*Product_{i=k+1..n}
 *          Sum_{j=0..i-1} A078008(j-1).
 * https://oeis.org/A128210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) enumerates the triangle T(n,k) given by T(n,k) = (-1)^(n-k)
 *            * ∏_{i=k+1..n} S(i) where S(i) = Σ_{j=0..i-1} A078008(j-1). The
 *            triangle values are read in row‑major order: a(0)=T(0,0),
 *            a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), … This implementation
 *            exploits that S(i) are the Jacobsthal numbers J(i...
 * Range:     i8 n <= 14, i16 n <= 27, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* floor(sqrt(x)) for 128‑bit unsigned integers */
static uint64_t isqrt_uint128(unsigned __int128 x) {
    uint64_t lo = 0;
    uint64_t hi = ~((uint64_t)0); /* max value of uint64_t */
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            if (mid == (uint64_t)-1) break;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* Static storage for pre‑computed Jacobsthal numbers and their products */
static seq_t *J = NULL;
static seq_t *P = NULL;
static idx_t  J_cap = 0;
static idx_t  P_cap = 0;
static idx_t  computed = 0;         /* highest index computed (≥0) */

/* Ensure J and P arrays have room up to 'need' and that values
   are computed up to that index. */
static void ensure_upto(idx_t need) {
    /* Initialize storage if first call */
    if (J_cap == 0) {
        J_cap = 2;
        P_cap = 2;
        J = (seq_t *)realloc(NULL, J_cap * sizeof(seq_t));
        P = (seq_t *)realloc(NULL, P_cap * sizeof(seq_t));
        J[0] = 0;
        J[1] = 1;
        P[0] = 1;
        P[1] = P[0] * J[1];
        computed = 1;
    }

    if (need <= computed) return;

    /* Grow arrays if necessary */
    if (J_cap <= need) {
        size_t new_cap = (size_t)(need + 1);
        J = (seq_t *)realloc(J, new_cap * sizeof(seq_t));
        J_cap = (idx_t)new_cap;
    }
    if (P_cap <= need) {
        size_t new_cap = (size_t)(need + 1);
        P = (seq_t *)realloc(P, new_cap * sizeof(seq_t));
        P_cap = (idx_t)new_cap;
    }

    /* Compute missing Jacobsthal values and cumulative products */
    for (idx_t i = computed + 1; i <= need; ++i) {
        J[i] = J[i-1] + (J[i-2] << 1);          /* J(i) = J(i-1) + 2*J(i-2) */
        P[i] = P[i-1] * J[i];                   /* P(i) = P(i-1) * J(i) */
    }
    computed = need;
}

/* Main function returning the nth term of A128210 */
seq_t a(idx_t n) {
    /* Find row r and column k such that n = r(r+1)/2 + k, 0 ≤ k ≤ r */
    unsigned __int128 disc = (unsigned __int128)8 * n + 1;
    idx_t r = (isqrt_uint128(disc) - 1) / 2;   /* floor((sqrt(8n+1)-1)/2) */

    unsigned __int128 tri = (unsigned __int128)r * (r + 1) / 2;
    idx_t start = (idx_t)tri;
    idx_t k = n - start;                       /* 0 ≤ k ≤ r */

    /* Ensure pre‑computed values up to row r */
    ensure_upto(r);

    /* Compute T(r,k) = (-1)^(r-k) * P[r] / P[k] */
    seq_t val = P[r] / P[k];
    if (((r - k) & 1ULL) != 0) {
        val = -val;                            /* apply sign */
    }
    return val;
}
