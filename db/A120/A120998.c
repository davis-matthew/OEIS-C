/*
 * A120998  Numerators of partial sums of Catalan numbers scaled by powers of
 *          1/7^2 = 1/49.
 * https://oeis.org/A120998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator( sum_{k=0}^n Catalan(k) / 49^k ) in lowest
 *            terms Recurrence: let P_n = Σ_{k=0}^n Catalan(k)*49^{n-k} P_0 =
 *            1 P_n = Catalan(n) + 49*P_{n-1} Then r(n) = P_n / 49^n, reduced
 *            by gcd(P_n,49^n). The numerator a(n) = P_n / gcd(P_n,49^n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-03)
 * Helpers:   catalan, gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/catalan.h"
#include "../utils/gcd.h"
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* external linkage required */
seq_t a(idx_t n)
{
    static seq_t *P = NULL;            /* array of P_n */
    static seq_t *A = NULL;            /* array of a(n) */
    static idx_t  stored  = 0;         /* number of elements stored (next index to compute) */
    static seq_t  denom   = 1;         /* 49^(stored-1)  */

    /* initialise for first call */
    if (stored == 0) {
        P = mem_new(seq_t, 1);
        A = mem_new(seq_t, 1);
        P[0] = catalan(0);          /* C_0 = 1 */
        A[0] = 1;                    /* a(0) = 1 */
        stored = 1;
        denom = 1;                   /* 49^0 */
    }

    /* grow arrays if n >= stored */
    if (n >= stored) {
        idx_t new_size = n + 1;
        P = mem_resize(seq_t, P, new_size);
        A = mem_resize(seq_t, A, new_size);

        for (idx_t i = stored; i <= n; ++i) {
            /* compute P_i = Catalan(i) + 49 * P_{i-1} */
            seq_t term = catalan(i);
            seq_t Pi   = term + mul_small(P[i-1], 49);
            P[i] = Pi;

            /* update denominator 49^i */
            denom = mul_small(denom, 49);

            /* reduce fraction and store numerator */
            seq_t g   = gcd(Pi, denom);
            A[i] = Pi / g;
        }
        stored = new_size;
    }

    return A[n];
}
