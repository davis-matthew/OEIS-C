/*
 * A000940  Number of n-gons with n vertices.
 * https://oeis.org/A000940
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Burnside over D_n x D_n acting on vertex orderings: a(n) =
 *            (1/(4n^2)) * Sum_{cycle types t} N(t)^2 * z(t), where N(t) is
 *            the number of elements of the dihedral group D_n with cycle type
 *            t and z(t) is the centralizer size in S_n. n odd, k=(n-1)/2:
 *            a(n) = ( Sum_{m|n} phi(m)^2 m^(n/m) (n/m)! + n^2 2...
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 15, u64 n <= 23, u128 n <= 36,
 *            big any n
 * Helpers:   a940_mulc, a940_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a940_mulc.h"
#include "../utils/a940_phi.h"

seq_t a(idx_t n)
{
    uint64_t D = 4 * (uint64_t)n * (uint64_t)n;
    seq_t Q = 0;
    uint64_t R = 0;
    uint64_t m, i, k;

    for (m = 1; m <= n; m++) {
        if (n % m != 0) continue;
        if (n % 2 == 0 && m == 2) continue;
        {
            seq_t q = 0;
            uint64_t r = 1;
            uint64_t ph = a940_phi(m), e = n / m;
            a940_mulc(&q, &r, ph, D);
            a940_mulc(&q, &r, ph, D);
            for (i = 0; i < e; i++) a940_mulc(&q, &r, m, D);
            for (i = 2; i <= e; i++) a940_mulc(&q, &r, i, D);
            Q += q;
            R += r;
        }
    }

    if (n % 2 == 1) {
        seq_t q = 0;
        uint64_t r = 1;
        k = (n - 1) / 2;
        a940_mulc(&q, &r, n, D);
        a940_mulc(&q, &r, n, D);
        for (i = 0; i < k; i++) a940_mulc(&q, &r, 2, D);
        for (i = 2; i <= k; i++) a940_mulc(&q, &r, i, D);
        Q += q;
        R += r;
    } else {
        seq_t q = 0;
        uint64_t r = 1;
        k = n / 2;
        a940_mulc(&q, &r, 1 + k, D);
        a940_mulc(&q, &r, 1 + k, D);
        for (i = 0; i < k; i++) a940_mulc(&q, &r, 2, D);
        for (i = 2; i <= k; i++) a940_mulc(&q, &r, i, D);
        Q += q;
        R += r;

        q = 0;
        r = 1;
        a940_mulc(&q, &r, k, D);
        a940_mulc(&q, &r, k, D);
        for (i = 0; i < k; i++) a940_mulc(&q, &r, 2, D);
        for (i = 2; i + 1 <= k; i++) a940_mulc(&q, &r, i, D);
        Q += q;
        R += r;
    }

    Q += (seq_t)(R / D);
    return Q;
}
