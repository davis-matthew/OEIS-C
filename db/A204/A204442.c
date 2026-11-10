/*
 * A204442  Permanent of the n-th principal submatrix of A204441.
 * https://oeis.org/A204442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * r_k * (n-k)!, where r_k are the
 *            rook numbers of the forbidden board (cells with i+j == 1 mod 4);
 *            the board is a disjoint union of four complete bipartite blocks
 *            K(c0,c1), K(c1,c0), K(c2,c3), K(c3,c2), c_m = #{i<=n: i%4==m},
 *            and block K(p,q) has rook numbers C(p,k)*C(q...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

/* rook polynomial of complete bipartite board K(p,q), length len, zero padded */
static seq_t *A204442_block(size_t p, size_t q, size_t len)
{
    seq_t *r = mem_new(seq_t, len);
    r[0] = 1;
    for (size_t k = 1; k < len && k <= p && k <= q; k++) {
        seq_t t = r[k - 1];
        t = t * (seq_t)(p - k + 1);
        t = t * (seq_t)(q - k + 1);
        t = t / (seq_t)k;
        r[k] = t;
    }
    return r;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    size_t len = N + 1;
    size_t c[4];
    c[0] = N / 4;
    c[1] = (N + 3) / 4;
    c[2] = (N + 2) / 4;
    c[3] = (N + 1) / 4;

    size_t P[4] = { c[0], c[1], c[2], c[3] };
    size_t Q[4] = { c[1], c[0], c[3], c[2] };

    seq_t *poly = mem_new(seq_t, len);
    poly[0] = 1;
    for (int b = 0; b < 4; b++) {
        seq_t *blk = A204442_block(P[b], Q[b], len);
        seq_t *res = mem_new(seq_t, len);
        for (size_t i = 0; i < len; i++) {
            if (poly[i] == 0) continue;
            for (size_t j = 0; i + j < len; j++) {
                if (blk[j] == 0) break;
                res[i + j] = res[i + j] + poly[i] * blk[j];
            }
        }
        mem_free(blk);
        mem_free(poly);
        poly = res;
    }

    seq_t *fact = mem_new(seq_t, len);
    fact[0] = 1;
    for (size_t i = 1; i < len; i++)
        fact[i] = fact[i - 1] * (seq_t)i;

    seq_t pos = 0, neg = 0;
    for (size_t k = 0; k < len; k++) {
        seq_t term = poly[k] * fact[N - k];
        if (k & 1) neg = neg + term;
        else pos = pos + term;
    }
    seq_t result = pos - neg;
    mem_free(poly);
    mem_free(fact);
    return result;
}
