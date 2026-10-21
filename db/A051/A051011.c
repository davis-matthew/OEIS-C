/*
 * A051011  Numerator of average of number of steps in Euclidean algorithm for
 *          all gcd(m,n) with 0<=m<n.
 * https://oeis.org/A051011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = gcd(S(n), n)⁻¹ * S(n), where * S(n) = Σ_{m=1}^{n-1}
 *            steps(m, n) * steps(m, n) = number of division steps in the
 *            Euclidean algorithm * for gcd(m, n) (m>0, 0≤m<n) * Steps are
 *            counted as the number of modulus operations until a zero *
 *            remainder is obtained. For example steps(1, 4) = 1, steps(...
 * Range:     u8 n <= 66, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _seq_gcd, _steps
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_seq_gcd.h"
#include "../utils/_steps.h"

/* --- state for incremental computation --------------------------- */
static idx_t _max_n = 0;          /* largest n computed so far          */
static seq_t * _S = NULL;         /* S[n] = sum of steps for that n    */

/* Expand internal tables to cover indices up to and including n. */
static void _extend_to(idx_t n)
{
    if (n <= _max_n) return;                  /* already done */

    _S = (seq_t *)realloc(_S, (size_t)(n + 1) * sizeof(seq_t));
    if (_S == NULL) abort();                  /* allocation failure */

    /* Compute S for each new n. */
    for (idx_t cur = _max_n + 1; cur <= n; ++cur) {
        seq_t sum = 0;
        for (idx_t m = 1; m < cur; ++m) {
            sum += _steps((seq_t)cur, (seq_t)m);
        }
        _S[cur] = sum;
    }
    _max_n = n;
}

/* --- exported function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* safety for out‑of‑range */
    _extend_to(n);
    seq_t s = _S[n];
    seq_t g = _seq_gcd(s, (seq_t)n);
    return s / g;                              /* numerator of reduced average */
}
