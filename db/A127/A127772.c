/*
 * A127772  Row sums of inverse of number triangle A(n,k) = 1/Euler_phi(n+1)
 *          if k <= n <= 2k, 0 otherwise.
 * https://oeis.org/A127772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = φ(n+1) - Σ_{m=⌈n/2⌉}^{n-1} a(m), a(0)=1
 * Range:     i8 n <= 59, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where φ denotes Euler's totient function. */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
#  define ALLOC_SEQ_T_ARRAY(ptr, sz) (ptr) = new seq_t[(sz)]
#  define FREE_SEQ_T_ARRAY(ptr)      delete[] (ptr)
#else
#  define ALLOC_SEQ_T_ARRAY(ptr, sz) (ptr) = (seq_t *)malloc((sz) * sizeof(seq_t))
#  define FREE_SEQ_T_ARRAY(ptr)      free((ptr))
#endif

seq_t a(idx_t n) {
    /* Allocate temporary arrays */
    seq_t *phi     = NULL;
    seq_t *s       = NULL;
    seq_t *prefix  = NULL;
    size_t nm1 = (size_t)n + 1;          /* largest index for φ needed */
    size_t len_phi   = nm1 + 1;          /* φ[0] .. φ[n+1]  */
    size_t len_s     = nm1;              /* rows 0..n       */

    ALLOC_SEQ_T_ARRAY(phi,   len_phi);
    ALLOC_SEQ_T_ARRAY(s,     len_s);
    ALLOC_SEQ_T_ARRAY(prefix, len_s);

    /* --- Compute φ(1)…φ(n+1) by sieve --- */
    for (size_t i = 0; i < len_phi; ++i)
        phi[i] = (seq_t)(i + 1);        /* φ(i+1) initialised to i+1 */

    for (size_t i = 2; i <= nm1; ++i) {
        if (phi[i-1] == (seq_t)i) {    /* i is prime */
            for (size_t j = i; j <= nm1; j += i) {
                phi[j-1] -= phi[j-1] / (seq_t)i;
            }
        }
    }

    /* --- Compute sequence values and prefix sums --- */
    for (size_t idx = 0; idx < len_s; ++idx) {
        seq_t h = phi[idx];                     /* φ(idx+1) */
        size_t L = (idx + 1) / 2;               /* ⌈n/2⌉ */
        seq_t sumPrev = 0;

        if (idx > 0 && L <= idx-1) {            /* sum s_L .. s_{idx-1} */
            seq_t prefR = prefix[idx-1];
            seq_t prefL = (L > 0) ? prefix[L-1] : 0;
            sumPrev = prefR - prefL;
        }

        seq_t val = h - sumPrev;
        s[idx] = val;
        if (idx == 0)
            prefix[0] = val;
        else
            prefix[idx] = prefix[idx-1] + val;
    }

    seq_t result = s[(size_t)n];

    /* Free temporary arrays */
    FREE_SEQ_T_ARRAY(phi);
    FREE_SEQ_T_ARRAY(s);
    FREE_SEQ_T_ARRAY(prefix);

    return result;
}
