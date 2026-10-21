/*
 * A203001  Symmetric matrix based on A007598, by antidiagonals.
 * https://oeis.org/A203001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry of the matrix M = T' * T read by
 *            antidiagonals. For a given n (offset 1): 1. Find d such that
 *            d(d+1)/2 ≥ n (d is the diagonal number). 2. Let i = n - (d-1)d/2
 *            (row index on that diagonal). 3. Let j = d - i + 1 (column index
 *            on that diagonal). 4. Then a(n) = Σ_{t=0}^{min(i,j)-1}...
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ----------  Fibonacci squares cache  ---------- */
static seq_t *fib_sq   = NULL;   /* s[k] = F_k^2  (1‑based) */
static seq_t *fib_fibo = NULL;   /* F_k           (1‑based)  */
static idx_t  fib_cap  = 0;      /* maximum k already cached */

/* Ensure fib_sq[1..need] and fib_fibo[1..need] are computed.  */
static void ensure_fib_sq(idx_t need)
{
    if (need <= fib_cap)
        return;

    idx_t new_cap = (fib_cap > 0) ? fib_cap * 2 : 2;
    if (new_cap < need)
        new_cap = need;

    /* Reallocate both arrays.  */
    fib_sq    = (seq_t *)realloc(fib_sq,    (new_cap + 1) * sizeof(seq_t));
    fib_fibo  = (seq_t *)realloc(fib_fibo,  (new_cap + 1) * sizeof(seq_t));
    if (!fib_sq || !fib_fibo)
        exit(1);                    /* unrecoverable */

    /* Prime the arrays if this is the first time.  */
    if (fib_cap == 0) {
        fib_fibo[0] = 0;            /* unused, but convenient */
        fib_fibo[1] = 1;
        fib_fibo[2] = 1;
        fib_sq[1]   = 1;
        fib_sq[2]   = 1;
        fib_cap     = 2;
    }

    /* Compute new Fibonacci numbers and their squares.  */
    for (idx_t k = fib_cap + 1; k <= new_cap; ++k) {
        fib_fibo[k] = fib_fibo[k-1] + fib_fibo[k-2];
        fib_sq[k]   = fib_fibo[k] * fib_fibo[k];
    }

    fib_cap = new_cap;
}

/* ----------  Diagonal finder  ---------- */
static idx_t diagonal_of(idx_t n)
{
    idx_t low  = 1, high = 1;

    /* Increase high until it satisfies the inequality.  */
    while (( (__int128)high * (high + 1) ) / 2 < n)
        high <<= 1;

    /* Binary search for the smallest high with high(high+1)/2 >= n.  */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (( (__int128)mid * (mid + 1) ) / 2 >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    return low;
}

/* ----------  Main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0)      /* undefined for offset 1, but harmless */
        return 0;

    /* Step 1 – locate the antidiagonal.  */
    idx_t d  = diagonal_of(n);                             /* 1‑based diagonal */
    idx_t pre = (d - 1) * d / 2;                           /* terms before diagonal d */
    idx_t pos = n - pre;                                   /* 1‑based position in this diagonal */
    idx_t i   = pos;                                       /* row index       */
    idx_t j   = d - pos + 1;                               /* column index    */

    /* At least we need Fibonacci squares up to max(i,j).  */
    idx_t need = (i > j) ? i : j;
    ensure_fib_sq(need);

    /* Step 2 – compute the matrix entry M[i][j].  */
    idx_t limit = (i < j) ? i : j;   /* min(i,j) */
    seq_t result = 0;

    for (idx_t t = 0; t < limit; ++t) {
        seq_t a1 = fib_sq[i - t];
        seq_t a2 = fib_sq[j - t];
        result += a1 * a2;
    }
    return result;
}
