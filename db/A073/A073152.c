/*
 * A073152  Triangle of numbers relating two simple context-free grammars
 *          (A052709 and A052705).
 * https://oeis.org/A073152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n,k) = \sum_{j=0}^{k} L[j] * L[n-j], L[j] = a(j,0).
 *            Recurrence: L[0] = 1, L[1] = 1, L[n] = D[n-1] + D[n-2] (n≥2),
 *            D[n] = \sum_{j=0}^{n} L[j] * L[n-j], a(n,0) = L[n], a(n,n) =
 *            D[n]. The requested OEIS sequence is the triangle printed
 *            row‑wise, so a(n) = a(row, col) where row is the largest
 *            integ...
 * Range:     u8 n <= 19, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   add_big, sum_mul_big, sum_partial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/add_big.h"
#include "../utils/sum_mul_big.h"
#include "../utils/sum_partial.h"

/* Triangle workspace */
static seq_t *L = NULL;          /* a(n,0) values */
static seq_t *D = NULL;          /* a(n,n) values */
static size_t cap = 0;           /* allocated capacity */
static size_t computed = 0;      /* number of rows already computed */

/* Multiply two seq_t values safely.
   In C builds we use unsigned __int128 for full precision.
   In C++ builds (for arbitrary‑precision types) we rely on the
   built‑in multiplication operator of seq_t. */
#if defined(__cplusplus)
static inline seq_t mul_big(seq_t a, seq_t b) { return a * b; }

#else
static inline seq_t mul_big(seq_t a, seq_t b) { return (seq_t)((unsigned __int128)a * b); }

#endif

/* Ensure that rows 0 … rmax have been computed. */
static void ensure_triangle(idx_t rmax)
{
    size_t need = (size_t)rmax + 1;
    if (need > cap) {
        size_t newcap = cap ? cap : 16;
        while (newcap < need) newcap <<= 1;
        L = (seq_t*)realloc(L, newcap * sizeof(seq_t));
        D = (seq_t*)realloc(D, newcap * sizeof(seq_t));
        cap = newcap;
    }

    if (computed == 0) {              /* initialise row 0 */
        L[0] = (seq_t)1;
        D[0] = (seq_t)1;
        computed = 1;
    }

    for (size_t n = computed; n < need; ++n) {
        if (n == 1) {                 /* L[1] = 1, D[1] from definition */
            L[1] = (seq_t)1;
            D[1] = sum_mul_big(L, 1);
            continue;
        }
        /* L[n] = D[n-1] + D[n-2] */
        L[n] = add_big(D[n-1], D[n-2]);

        /* D[n] = sum_{j=0}^{n} L[j] * L[n-j] */
        D[n] = sum_mul_big(L, n);
    }
    computed = need;
}

/* Public function: a(n) – the n‑th element of the flattened triangle. */
seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;

    idx_t col = n - r * (r + 1) / 2;    /* column within the row */

    ensure_triangle(r);

    /* a(r,col) = sum_{j=0}^{col} L[j] * L[r-j] */
    return sum_partial(L, r, col);
}
