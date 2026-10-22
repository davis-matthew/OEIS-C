/*
 * A000258  Expansion of e.g.f. exp(exp(exp(x)-1)-1).
 * https://oeis.org/A000258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=0..n} Stirling2(n,k)*Bell(k), computed as a(n) =
 *            (D^n Bell)(0) with (D c)(k) = k*c(k) + c(k+1); Bell(k) from the
 *            Bell (Aitken) triangle
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a258_cache = 0;
static idx_t a258_have = 0; /* number of cached terms */

static void a258_build(idx_t N)
{
    idx_t r, j, k, m;
    seq_t *bell = (seq_t *)calloc((size_t)N + 2, sizeof(seq_t));
    seq_t *t = (seq_t *)calloc((size_t)N + 2, sizeof(seq_t));
    seq_t *res = (seq_t *)calloc((size_t)N + 2, sizeof(seq_t));

    /* Bell triangle, in place: row r has r+1 entries, Bell(r) = first entry */
    t[0] = 1;
    bell[0] = 1;
    for (r = 1; r <= N; r++) {
        seq_t cur = t[r - 1];
        for (j = 0; j < r; j++) {
            seq_t o = t[j];
            t[j] = cur;
            cur += o;
        }
        t[r] = cur;
        bell[r] = t[0];
    }

    /* Stirling transform: repeatedly apply c(k) <- k*c(k) + c(k+1) */
    for (m = 0; m <= N; m++) {
        res[m] = bell[0];
        for (k = 0; k + m < N; k++)
            bell[k] = bell[k] * k + bell[k + 1];
    }

    free(bell);
    free(t);
    if (a258_cache) free(a258_cache);
    a258_cache = res;
    a258_have = N + 1;
}

seq_t a(idx_t n)
{
    if (n >= a258_have) {
        idx_t N = n;
        if (a258_have > 0 && N < 2 * a258_have) N = 2 * a258_have;
        if (N < 32) N = 32;
        a258_build(N);
    }
    return a258_cache[n];
}
