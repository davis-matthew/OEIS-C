/*
 * A078516  Sum of balls on the lawn for the s=4 tennis ball problem.
 * https://oeis.org/A078516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((4*n^2+11*n+8)*A002293(n+1) - A078995(n+1))/2,
 *            A002293(n)=binomial(4n,n)/(3n+1), A078995(n)=Sum_{k=0..n}
 *            binomial(4k,k)*binomial(4(n-k),n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-07)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

static uint64_t A002293_leg(uint64_t m, uint64_t p) {
    uint64_t e = 0;
    while (m) { m /= p; e += m; }
    return e;
}

static seq_t A002293_a(idx_t n) {
    seq_t r = 1;
    uint64_t N, i, j, p;
    unsigned char *comp;
    if (n == 0) return r;
    N = 4 * (uint64_t)n;
    comp = (unsigned char *)calloc((size_t)N + 1, 1);
    for (i = 2; i <= N; i++) {
        if (comp[i]) continue;
        for (j = i * i; j <= N; j += i) comp[j] = 1;
        p = i;
        {
            uint64_t e = A002293_leg(N, p) - A002293_leg((uint64_t)n, p)
                       - A002293_leg(3 * (uint64_t)n + 1, p);
            while (e--) r *= (seq_t)p;
        }
    }
    free(comp);
    return r;
}

/* Half of A078995(m), computed without overflow of the doubled value. */
static seq_t A078995_half(idx_t m) {
    seq_t *b = mem_new(seq_t, (size_t)m + 1);
    seq_t s = 0;
    size_t k;
    for (k = 0; k <= (size_t)m; ++k) b[k] = binomial((idx_t)(4 * k), (idx_t)k);
    for (k = 0; 2 * k < (size_t)m; ++k) s += b[k] * b[(size_t)m - k];
    if ((m & 1) == 0) {
        seq_t h = b[(size_t)m / 2];
        s += h * (h / 2);
    }
    mem_free(b);
    return s;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t P = 4 * nn * nn + 11 * nn + 8;
    seq_t C = A002293_a(n + 1);
    seq_t H = A078995_half(n + 1);
    seq_t t;
    if ((P & 1) == 0) t = (P / 2) * C;
    else t = (C / 2) * P;
    return t - H;
}
