/*
 * A213607  Expansion of psi(x^4) * f(-x^3)^3 / f(-x) in powers of x where
 *          psi(), f() are Ramanujan theta functions.
 * https://oeis.org/A213607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A213607 – Euler transform of the periodic sequence
    B(k) = [1,1,-2,2,1,-2,1,0,-2,1,1,-1,1,1,-2,0,1,-2,1,2,-2,1,1,-3]
    a(0) = 1
    For n ≥ 1:
        a(n) = (1/n) * Σ_{k=1..n} ( Σ_{d|k} d*B(d) ) * a(n-k)
    The sieve below recomputes the divisor‑sum array S(k) up to n each call.
    For n ≤ 10 000 this is well below the two‑second per‑call limit.   */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *A   = NULL;      /* sequence values      */
    static size_t capA = 0;        /* array capacity       */
    static size_t lenA = 0;        /* number of filled entries */

    static int *S   = NULL;        /* divisor‑sum array     */
    static size_t capS = 0;        /* capacity of S         */

    /* period‑24 sequence B */
    static const int B24[24] = {
        1,  1, -2,  2,  1, -2,  1,  0,
       -2,  1,  1, -1,  1,  1, -2,  0,
        1, -2,  1,  2, -2,  1,  1, -3
    };

    /* ---- handle base case n == 0 --------------------------------- */
    if (n == 0) {
        if (lenA == 0) {                     /* initialise A[0] */
            A = (seq_t*)realloc(A, sizeof(seq_t));
            if (!A) return 0;                /* allocation failure */
            A[0]  = (seq_t)1;
            lenA  = 1;
            capA  = 1;
        }
        return A[0];
    }

    /* ---- ensure capacity for A and S up to index n ---------------- */
    size_t target = (size_t)n;
    if (target + 1 > capA) {
        seq_t *tmp = (seq_t*)realloc(A, (target + 1) * sizeof(seq_t));
        if (!tmp) return 0;
        A = tmp;
        capA = target + 1;
    }
    if (target + 1 > capS) {
        int *tmp = (int*)realloc(S, (target + 1) * sizeof(int));
        if (!tmp) return 0;
        S = tmp;
        capS = target + 1;
    }

    /* ---- initialise divisor‑sum array S(k) ------------------------ */
    for (size_t i = 0; i <= target; ++i) S[i] = 0;
    for (int d = 1; d <= (int)target; ++d) {
        int val = d * B24[(d - 1) % 24];
        if (val == 0) continue;
        for (int m = d; m <= (int)target; m += d)
            S[m] += val;
    }

    /* ---- initialise A[0] if not already done -------------------- */
    if (lenA == 0) {
        A[0] = (seq_t)1;
        lenA = 1;
    }

    /* ---- compute a(k) for all required k ------------------------ */
    for (idx_t idx = (idx_t)lenA; idx <= n; ++idx) {
        __int128 sum = 0;
        for (idx_t k = 1; k <= idx; ++k) {
            int Sk = S[(size_t)k];
            if (!Sk) continue;
            sum += (__int128)Sk * (__int128)A[(size_t)(idx - k)];
        }
        A[(size_t)idx] = (seq_t)(sum / idx);  /* integer division */
    }
    lenA = target + 1;

    return A[(size_t)n];
}
