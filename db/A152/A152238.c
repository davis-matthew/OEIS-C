/*
 * A152238  A modulo two parity function as a triangle sequence:k=2;
 *          t(n,m)=Binomial[n,m]+p(n,m); Always even parity function:
 *          p(n,m)=If[Mod[Binomial[n, m], 2] == 0, 2^(k - 1)*Binomial[n, m],
 *          If[Mod[Binomial[n, m], 2] == 1 && Binomial[n, m] > 1, 2^k*
 *          Binomial[n, m], 0]].
 * https://oeis.org/A152238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A152238 – flattened triangle t(n,m)
 * 
 * The triangle is defined by
 *   t(n,m) = Binomial(n,m) + p(n,m),
 *   p(n,m) = { 2 * Binomial(n,m)          if Binomial(n,m) even,
 *              4 * Binomial(n,m)          if Binomial(n,m) odd and > 1,
 *              0                          otherwise }.
 * k = 2 in the definition above.
 *
 * The sequence a(n) is the list of the triangle entries read row‑wise:
 *   n = 0   →  (r=0,c=0)
 *   n = 1   →  (r=1,c=0)
 *   n = 2   →  (r=1,c=1)
 *   n = 3   →  (r=2,c=0)
 *   n = 4   →  (r=2,c=1)
 *   …
 *
 * For a given index n, the corresponding row r and column c are
 *   r = ⌊ (√(8n+1) – 1) / 2 ⌋
 *   c = n – r(r+1)/2
 *
 * The function below computes a(n) for any n ≥ 0, using only integer
 * arithmetic and the helper routine binomial().  It runs in O(min(c,r‑c))
 * time which is below a few microseconds for n ≤ 10 000. */

#include <math.h>
#include <stdint.h>

static inline seq_t binomial(idx_t n, idx_t k)
{
    /* Compute C(n,k) using the multiplicative formula.
       k may be replaced by n‑k if it is larger. */
    idx_t kk = k;
    if (kk > n - kk)
        kk = n - kk;
    seq_t res = 1;
    for (idx_t i = 1; i <= kk; ++i) {
        res *= (seq_t)(n - kk + i);
        res /= (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row r (0‑based) such that (r+1)(r+2)/2 > n. */
    idx_t r = (idx_t)((sqrt((double)(8ULL * (unsigned long long)n + 1ULL)) - 1.0) / 2.0);
    /* Cumulated count of elements up to row r‑1: r(r+1)/2. */
    idx_t prev_count = r * (r + 1) / 2;
    /* Column index c within the current row. */
    idx_t c = n - prev_count;

    seq_t bin = binomial(r, c);

    /* t = bin + p: shorthand as described above. */
    if (bin == 1)
        return bin;                 /* holds for all bin = 1 cases */
    if (bin & 1)                      /* bin odd and > 1 */
        return bin * (seq_t)5;
    else                              /* bin even */
        return bin * (seq_t)3;
}
