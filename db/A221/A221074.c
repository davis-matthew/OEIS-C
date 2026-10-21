/*
 * A221074  Simple continued fraction expansion of product {n >= 0} {1 -
 *          sqrt(m)*[sqrt(m) - sqrt(m-1)]^(4*n+3)}/{1 - sqrt(m)*[sqrt(m) -
 *          sqrt(m-1)]^(4*n+1)} at m = 3.
 * https://oeis.org/A221074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cofr
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 36, u64 n <= 76, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A221074: simple continued fraction expansion of
 * ∏_{n≥0} (1‑√3·(√3‑√2)^{4n+3})/(1‑√3·(√3‑√2)^{4n+1}) at m=3.
 *
 *  The terms are given by the formulas
 *
 *    a(0) = 2
 *    a(2k) = 1      for k ≥ 1
 *
 *    For k ≥ 1
 *      a(4k-3) = (√3+√2)^{2k} + (√3-√2)^{2k} - 2
 *      a(4k-1) = (1/√3)[ (√3+√2)^{2k+1} + (√3-√2)^{2k+1} ] - 2
 *
 *  These expressions evaluate to integers.  Let
 *
 *      U_k = (√3+√2)^{2k} + (√3-√2)^{2k}      (integer)
 *      V_k = (1/√3)[ (√3+√2)^{2k+1} + (√3-√2)^{2k+1} ]  (integer)
 *
 *  Then
 *
 *      a(4k-3) = U_k - 2
 *      a(4k-1) = V_k - 2
 *
 *  The sequences U_k and V_k satisfy the simple recurrences
 *
 *      U_1 = 10,   V_1 = 18
 *      U_{k+1} = 6·V_k - U_k
 *      V_{k+1} = 2·U_{k+1} - V_k
 *
 *  (derived from the linear recurrence for A_m = (√3+√2)^m +(√3-√2)^m).
 *  The recurrence uses only integer arithmetic, so it can be evaluated
 *  for arbitrarily large k.  The implementation keeps a static cache
 *  of U_k and V_k so that calls to a() with arbitrary n are answered
 *  in O(n) time overall, with very small constant factors.
 */

#if defined(__cplusplus)
# include <vector>
#else
# include <stdlib.h>
# include <stddef.h>
#endif

/* Static cache of U_k and V_k */
#if defined(__cplusplus)
static std::vector<seq_t> U_vec;
static std::vector<seq_t> V_vec;
#else
static seq_t *U = NULL;
static seq_t *V = NULL;
#endif
static idx_t maxK = 0;   /* largest k for which U_k and V_k are known */

/* Ensure that U_k and V_k are computed for all k up to k_needed. */
static void ensure_capacity(idx_t k_needed) {
#if defined(__cplusplus)
    if (maxK >= k_needed) return;

    /* Initialise base values if this is the first call */
    if (maxK == 0) {
        U_vec.resize(2);          /* indices 0 and 1; 0 unused */
        V_vec.resize(2);
        U_vec[1] = 10;            /* U_1 */
        V_vec[1] = 18;            /* V_1 */
        maxK   = 1;
    }

    if (U_vec.size() <= k_needed) {
        U_vec.resize(k_needed + 1);
        V_vec.resize(k_needed + 1);
    }

    for (idx_t i = maxK + 1; i <= k_needed; ++i) {
        seq_t u_prev = U_vec[i-1];
        seq_t v_prev = V_vec[i-1];
        seq_t u_curr = 6 * v_prev - u_prev;
        seq_t v_curr = 2 * u_curr - v_prev;
        U_vec[i] = u_curr;
        V_vec[i] = v_curr;
    }
    maxK = k_needed;
#else
    if (maxK >= k_needed) return;

    /* Allocate enough storage (extra space for future growth). */
    size_t new_cap = k_needed + 10;
    U = (seq_t*)realloc(U, new_cap * sizeof(seq_t));
    V = (seq_t*)realloc(V, new_cap * sizeof(seq_t));

    /* Initialise base values if this is the first call */
    if (maxK == 0) {
        U[1] = 10;            /* U_1 */
        V[1] = 18;            /* V_1 */
        maxK = 1;
    }

    for (idx_t i = maxK + 1; i <= k_needed; ++i) {
        seq_t u_prev = U[i-1];
        seq_t v_prev = V[i-1];
        seq_t u_curr = 6 * v_prev - u_prev;
        seq_t v_curr = 2 * u_curr - v_prev;
        U[i] = u_curr;
        V[i] = v_curr;
    }
    maxK = k_needed;
#endif
}

/* Public sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 2;                      /* a(0) = 2 */

    if ((n & 1) == 0) return 1;                /* a(2k) = 1 for k≥1 */

    /* n is odd */
    if ((n & 3) == 1) {     /* n ≡ 1 (mod 4) -> n = 4k-3 */
        idx_t k = (n + 3) / 4;   /* k ≥ 1 */
        ensure_capacity(k);
#if defined(__cplusplus)
        return U_vec[k] - 2;
#else
        return U[k] - 2;
#endif
    } else {                 /* n ≡ 3 (mod 4) -> n = 4k-1 */
        idx_t k = (n + 1) / 4;   /* k ≥ 1 */
        ensure_capacity(k);
#if defined(__cplusplus)
        return V_vec[k] - 2;
#else
        return V[k] - 2;
#endif
    }
}
