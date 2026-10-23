/*
 * A024888  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = A023531, t = A001950 (upper Wythoff sequence).
 * https://oeis.org/A024888
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊n/2⌋} s(i) · t(n‑i+1)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where s(i) = 1 if (i+1) is a triangular number,      */
/* otherwise s(i) = 0;  t(m) = floor(m·φ²)  (upper Wythoff sequence). */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* ---------- cache for the upper Wythoff sequence t(n) ---------- */
static seq_t *T = NULL;          /* 1‑based: T[i] = t(i) */
static idx_t  Tsize = 0;         /* number of precomputed terms */

/* ---------- cache for the indicator sequence s(i) ---------- */
static unsigned char *S = NULL;  /* 1‑based: S[i] = 0 or 1 */
static idx_t  Ssize = 0;         /* number of precomputed terms */

/* ---------- Build t[1..n] (upper Wythoff sequence) ---------- */
static void build_t(idx_t n)
{
    if (Tsize >= n) return;

    if (!T)
        T = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    else
        T = (seq_t *)realloc(T, (n + 1) * sizeof(seq_t));

    if (!T) abort();

    /* compute missing terms */
    for (idx_t i = Tsize + 1; i <= n; ++i) {
        seq_t ti   = (seq_t)i;            /* i as seq_t */
        seq_t sq   = ti * ti;             /* i²   */
        sq *= (seq_t)5;                   /* 5·i² */
        seq_t sr   = floor_sqrt(sq);      /* floor(√(5·i²)) == floor(√5·i) */
        T[i] = ((seq_t)3 * ti + sr) / 2;  /* t(i) = floor((3i + sr)/2) */
    }
    Tsize = n;
}

/* ---------- Build s[1..k] (indicator of (i+1) triangular) ---------- */
static void build_s(idx_t k)
{
    if (Ssize >= k) return;

    if (!S)
        S = (unsigned char *)malloc((k + 1) * sizeof(unsigned char));
    else
        S = (unsigned char *)realloc(S, (k + 1) * sizeof(unsigned char));

    if (!S) abort();

    /* generate triangular numbers on the fly */
    idx_t m   = 2;                /* start from T₂ = 3 */
    idx_t tri = (m * (m + 1)) / 2;/* first triangular number > 1 */

    for (idx_t i = 1; i <= k; ++i) {
        if (i + 1 == tri) {
            S[i] = 1;
            m++;
            tri = (m * (m + 1)) / 2;
        } else {
            S[i] = 0;
        }
    }
    Ssize = k;
}

/* ---------- public sequence routine ---------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0; /* undefined for n < 2, return 0 as a safe default */

    build_t(n);
    build_s(n >> 1);             /* k = floor(n/2) */

    idx_t k   = n >> 1;
    seq_t sum = (seq_t)0;

    for (idx_t i = 1; i <= k; ++i) {
        if (S[i]) {
            sum += T[n - i + 1];
        }
    }
    return sum;
}
