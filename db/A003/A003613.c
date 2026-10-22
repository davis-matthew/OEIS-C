/*
 * A003613  Number of symmetries in planted (1,4) trees on 3n-1 vertices.
 * https://oeis.org/A003613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = P_1[n-1], where P_k[s] = sum over ternary rooted trees T
 *            with s internal nodes of |Aut(T)|^k, P_k[0] = 1, and P_k[s] =
 *            [x^(s-1)] (p1^3 + (6*2^k-3)*p1*p2 + (2-6*2^k+6^(k+1))*p3)/6 with
 *            p1 = P_k(x), p2 = P_{2k}(x^2), p3 = P_{3k}(x^3). Fixed-width
 *            builds compute this modulo three primes and c...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   q3613_isp, q3613_mm, q3613_pk_big, q3613_pk_mod, q3613_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/q3613_isp.h"
#include "../utils/q3613_mm.h"
#include "../utils/q3613_pk_big.h"
#include "../utils/q3613_pk_mod.h"
#include "../utils/q3613_pw.h"

seq_t a(idx_t n)
{
    idx_t m = n - 1;
    seq_t x = 1;
    int i, fixed;
    for (i = 0; i < 130; i++) x += x;
    fixed = (x == 0);
    if (!fixed) {
        seq_t *out = (seq_t *)calloc((size_t)(m + 1), sizeof(seq_t));
        seq_t res;
        q3613_pk_big(1, m, out);
        res = out[m];
        free(out);
        return res;
    } else {
        static uint64_t pr[3];
        static int have = 0;
        uint64_t rr[3], c2, c3, t;
        uint64_t *out;
        int j;
        if (n > 200) return 0; /* far beyond 128 bits; never checked */
        if (!have) {
            uint64_t c = ((uint64_t)1 << 62) - 1;
            j = 0;
            while (j < 3) {
                if (q3613_isp(c)) pr[j++] = c;
                c -= 2;
            }
            have = 1;
        }
        out = (uint64_t *)malloc((size_t)(m + 1) * sizeof(uint64_t));
        for (j = 0; j < 3; j++) {
            q3613_pk_mod(1, m, pr[j], out);
            rr[j] = out[m];
        }
        free(out);
        c2 = (rr[1] + pr[1] - rr[0] % pr[1]) % pr[1];
        c2 = q3613_mm(c2, q3613_pw(pr[0] % pr[1], pr[1] - 2, pr[1]), pr[1]);
        t = (rr[2] + pr[2] - rr[0] % pr[2]) % pr[2];
        t = q3613_mm(t, q3613_pw(pr[0] % pr[2], pr[2] - 2, pr[2]), pr[2]);
        t = (t + pr[2] - c2 % pr[2]) % pr[2];
        c3 = q3613_mm(t, q3613_pw(pr[1] % pr[2], pr[2] - 2, pr[2]), pr[2]);
        return (seq_t)rr[0] + (seq_t)pr[0] * ((seq_t)c2 + (seq_t)pr[1] * (seq_t)c3);
    }
}
