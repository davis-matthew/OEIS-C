/*
 * A094156  Bisection of A000940.
 * https://oeis.org/A094156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000940(2n+1), where A000940(N) = (1/(4N^2)) * (Sum_{m|N}
 *            phi(m)^2 m^(N/m) (N/m)! + N^2 2^k k!) for odd N, k=(N-1)/2
 *            (even-N terms are never used)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-07)
 * Helpers:   totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_u64.h"

static void A094156_mulc(seq_t *q, uint64_t *r, uint64_t c, uint64_t D)
{
    unsigned __int128 t = (unsigned __int128)(*r) * c;
    *q = *q * c + (seq_t)(uint64_t)(t / D);
    *r = (uint64_t)(t % D);
}

seq_t a(idx_t idx)
{
    uint64_t n = 2 * (uint64_t)idx + 1;
    uint64_t D = 4 * n * n;
    seq_t Q = 0;
    uint64_t R = 0;
    uint64_t m, i, k;

    for (m = 1; m <= n; m++) {
        if (n % m != 0) continue;
        {
            seq_t q = 0;
            uint64_t r = 1;
            uint64_t ph = totient_u64(m), e = n / m;
            A094156_mulc(&q, &r, ph, D);
            A094156_mulc(&q, &r, ph, D);
            for (i = 0; i < e; i++) A094156_mulc(&q, &r, m, D);
            for (i = 2; i <= e; i++) A094156_mulc(&q, &r, i, D);
            Q += q;
            R += r;
        }
    }

    {
        seq_t q = 0;
        uint64_t r = 1;
        k = (n - 1) / 2;
        A094156_mulc(&q, &r, n, D);
        A094156_mulc(&q, &r, n, D);
        for (i = 0; i < k; i++) A094156_mulc(&q, &r, 2, D);
        for (i = 2; i <= k; i++) A094156_mulc(&q, &r, i, D);
        Q += q;
        R += r;
    }

    Q += (seq_t)(R / D);
    return Q;
}
