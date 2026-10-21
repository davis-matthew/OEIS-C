/*
 * A052207  Number of sequences {s(i): i=0..n} such that |s(i)-s(i-1)|=1,
 *          i=1..n and s(i)=0 at four values of i, one of which is i=0.
 * https://oeis.org/A052207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8 * Σ_{m=0}^{⌊(n‑6)/2⌋} (C_m)^3 * f(n‑6‑2m) where C_m is
 *            the m‑th Catalan number and f(L) = 1 if L = 0 else 2 *
 *            binom(L‑1, ⌊(L‑1)/2⌋)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"


/* Catalan numbers and their convolutions. */
static idx_t cat_size = 0;         /* number of Catalan entries stored */
static seq_t *cat = NULL;          /* C[0..cat_size-1]                  */
static seq_t *cat2 = NULL;         /* C * C convolution                 */
static seq_t *cat3 = NULL;         /* C * C * C convolution             */

static void ensure_precompute(idx_t n)
{
    if (n < 6) return;

    size_t maxM = (size_t)((n - 6) / 2);
    if (maxM < cat_size) return;          /* already enough */

    size_t newSize = maxM + 1;

    /* Resize arrays. */
    cat  = (seq_t *)realloc(cat,  newSize * sizeof(seq_t));
    cat2 = (seq_t *)realloc(cat2, newSize * sizeof(seq_t));
    cat3 = (seq_t *)realloc(cat3, newSize * sizeof(seq_t));
    if (!cat || !cat2 || !cat3) abort();

    /* Compute new Catalan numbers. */
    for (size_t k = cat_size; k < newSize; ++k) {
        idx_t kk = (idx_t)k;
        seq_t val = binom(2 * kk, kk);   /* (2k choose k) */
        cat[k] = val / (seq_t)(kk + 1);  /* Catalan(k)    */
    }

    /* Convolution C * C = cat2. */
    for (size_t k = 0; k < newSize; ++k) {
        seq_t sum = 0;
        for (size_t i = 0; i <= k; ++i)
            sum += cat[i] * cat[k - i];
        cat2[k] = sum;
    }

    /* Convolution (C * C) * C = cat3. */
    for (size_t k = 0; k < newSize; ++k) {
        seq_t sum = 0;
        for (size_t i = 0; i <= k; ++i)
            sum += cat2[i] * cat[k - i];
        cat3[k] = sum;
    }

    cat_size = newSize;
}

static seq_t zero_avoiding_tail(idx_t L)
{
    if (L == 0) return (seq_t)1;
    idx_t L1 = L - 1;                     /* steps after the first one */
    idx_t half = L1 / 2;
    seq_t bin = binom(L1, half);
    return bin * (seq_t)2;
}

seq_t a(idx_t n)
{
    if (n < 6) return (seq_t)0;

    ensure_precompute(n);

    seq_t result = 0;
    size_t maxM = (size_t)((n - 6) / 2);

    for (size_t m = 0; m <= maxM; ++m) {
        idx_t L = n - 6 - (idx_t)(2 * m);
        seq_t tail = zero_avoiding_tail(L);
        seq_t term = cat3[m] * tail;
        term *= (seq_t)8;
        result += term;
    }

    return result;
}
