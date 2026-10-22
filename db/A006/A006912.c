/*
 * A006912  Theta series of laminated lattice LAMBDA_12^{min}.
 * https://oeis.org/A006912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = P^6 + 600*P^4*Q^2 + 3456*P^3*Q^3 + 7680*P^2*Q^4 +
 *            6144*P*Q^5 - 2048*Q^6, where P = theta_3(q^2)^2 and Q =
 *            q*psi(q^4)^2, with theta_3(q) = Sum_{k in Z} q^(k^2) and psi(q)
 *            = Sum_{k>=0} q^(k(k+1)/2). This is a weight-6 form on Gamma_0(8)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 47, u64 n <= 4043,
 *            u128 n <= 9250, big any n
 * Helpers:   A006912_mulG, A006912_mulh, from_i128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/from_i128.h"
#include "../utils/A006912_mulG.h"
#include "../utils/A006912_mulh.h"

static __int128 *A006912_cache = NULL;
static long long A006912_cap = 0;

static void A006912_build(long long L)
{
    static const int c[7] = {1, 0, 600, 3456, 7680, 6144, -2048};
    __int128 *S = (__int128 *)calloc((size_t)L, sizeof(__int128));
    __int128 *Qp = (__int128 *)calloc((size_t)L, sizeof(__int128));
    long long i;
    int j;
    S[0] = c[0];
    Qp[0] = 1;
    for (j = 1; j <= 6; j++) {
        /* S *= P */
        A006912_mulG(S, L);
        A006912_mulG(S, L);
        /* Qp *= Q */
        A006912_mulh(Qp, L);
        A006912_mulh(Qp, L);
        for (i = L - 1; i >= 1; i--)
            Qp[i] = Qp[i - 1];
        Qp[0] = 0;
        if (c[j] != 0)
            for (i = 0; i < L; i++)
                S[i] += (__int128)c[j] * Qp[i];
    }
    free(Qp);
    free(A006912_cache);
    A006912_cache = S;
    A006912_cap = L;
}

seq_t a(idx_t n)
{
    if ((long long)n >= A006912_cap) {
        long long L = 2 * A006912_cap;
        if (L < (long long)n + 1)
            L = (long long)n + 1;
        if (L < 64)
            L = 64;
        A006912_build(L);
    }
    return from_i128(A006912_cache[n]);
}
