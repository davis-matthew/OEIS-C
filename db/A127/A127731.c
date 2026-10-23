/*
 * A127731  Triangle read by rows, where row n consists of the r's where r =
 *          (n*m)/(n+m) and the m's are positive integers such that (n+m)
 *          divides (n*m).
 * https://oeis.org/A127731
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A127731: Triangle read by rows.
   r = n - n^2/d where d runs over the divisors of n^2 greater than n.
   a(n) is the (n‑2)-th element of the flattened triangle starting at offset 2. */
#include <stdint.h>
#include <stdlib.h>

/* Simple factorisation: populates arrays with primes and exponents of x. */
static void factor_uint64(idx_t x, idx_t *primes, int *exps, size_t *cnt)
{
    size_t c = 0;
    for (idx_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            int e = 0;
            while (x % p == 0)
            {
                x /= p;
                ++e;
            }
            primes[c] = p;
            exps[c] = e;
            ++c;
        }
    }
    if (x > 1)
    {
        primes[c] = x;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Recursively generate all divisors of n^2 from prime factorisation. */
static void gen_divs_rec(idx_t idx, idx_t cur, const idx_t *primes,
                         const int *exps, size_t pcnt, idx_t *out,
                         size_t *outcnt)
{
    if (idx == pcnt)
    {
        out[*outcnt] = cur;
        ++(*outcnt);
        return;
    }
    idx_t p = primes[idx];
    idx_t power = 1;
    for (int e = 0; e <= 2 * exps[idx]; ++e)
    {
        gen_divs_rec(idx + 1, cur * power, primes, exps,
                     pcnt, out, outcnt);
        power *= p;
    }
}

/* Sort comparator for qsort. */
static int cmp_idx_t(const void *a, const void *b)
{
    idx_t aa = *(const idx_t *)a;
    idx_t bb = *(const idx_t *)b;
    return (aa < bb) ? -1 : (aa > bb);
}

/* Number of terms in row n = (τ(n^2)-1)/2 where τ = number of divisors. */
static idx_t row_length(idx_t n)
{
    idx_t primes[64];
    int exps[64];
    size_t pcnt;
    factor_uint64(n, primes, exps, &pcnt);

    idx_t dcnt = 1;
    for (size_t i = 0; i < pcnt; ++i)
        dcnt *= (2 * exps[i] + 1);

    return (dcnt - 1) / 2;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;               /* below offset */

    idx_t target = n - 2;               /* position in the flattened triangle (0‑based) */
    idx_t row = 2;                      /* start from row 2 */

    /* Find the row containing the target. */
    while (1)
    {
        idx_t len = row_length(row);
        if (target < len)
            break;
        target -= len;
        ++row;
    }

    /* Generate all divisors of row^2. */
    idx_t primes[64];
    int exps[64];
    size_t pcnt;
    factor_uint64(row, primes, exps, &pcnt);

    size_t total_divs = 1;
    for (size_t i = 0; i < pcnt; ++i)
        total_divs *= (2 * exps[i] + 1);

    idx_t *divs = (idx_t *)malloc(total_divs * sizeof(idx_t));
    size_t divcnt = 0;
    gen_divs_rec(0, 1, primes, exps, pcnt, divs, &divcnt);

    /* Keep only divisors > row. */
    idx_t *filtered = (idx_t *)malloc(divcnt * sizeof(idx_t));
    size_t filtcnt = 0;
    for (size_t i = 0; i < divcnt; ++i)
        if (divs[i] > row)
            filtered[filtcnt++] = divs[i];

    free(divs);

    /* Sort the filtered divisors increasing. */
    qsort(filtered, filtcnt, sizeof(idx_t), cmp_idx_t);

    /* target is zero‑based, so pick filtered[target]. */
    idx_t d = filtered[target];
    free(filtered);

    seq_t n_seq = (seq_t)row;
    seq_t n_sq  = n_seq * n_seq;
    seq_t r = n_seq - n_sq / (seq_t)d;
    return r;
}
