/*
 * A069321  Stirling transform of A001563: a(0) = 1 and a(n) = Sum_{k=1..n}
 *          Stirling2(n,k)*k*k! for n >= 1.
 * https://oeis.org/A069321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A069321 – Stirling transform of A001563
 *
 * a(0) = 1
 * a(n) = Sum_{k=1..n} Stirling2(n,k) * k * k!  (n >= 1)
 *
 * Using the identity
 *   a(n) = (B[n+1] - B[n]) / 2
 * where B[m] is the ordered Bell number (A000670):
 *   B[0] = 1
 *   B[m] = Sum_{k=0}^{m-1} binom(m,k) * B[k] (m >= 1)
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *B      = NULL;   /* array of B[0..max]        */
static size_t B_cap  = 0;      /* allocated capacity of B   */
static idx_t  B_max  = 0;      /* maximum index already computed */

/* ensure B[0..n] are computed */
static void ensure_B(idx_t n)
{
    if (n <= B_max)
        return;                 /* already computed */

    /* enlarge capacity if needed */
    size_t new_cap = B_cap;
    if (new_cap == 0) new_cap = 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    if (new_cap > B_cap) {
        seq_t *tmp = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();           /* allocation failed */
        B = tmp;
        B_cap = new_cap;
    }

    /* initialise the first element if this is the very first run */
    if (B_max == 0) {
        B[0] = (seq_t)1;
    }

    /* compute new entries using the recurrence */
    for (idx_t m = B_max + 1; m <= n; ++m) {
        seq_t sum  = (seq_t)0;
        seq_t binom = (seq_t)1;      /* binom(m,0) */
        for (idx_t k = 0; k < m; ++k) {
            sum += binom * B[k];
            /* update binom to binom(m,k+1) */
            binom = binom * (seq_t)(m - k) / (seq_t)(k + 1);
        }
        B[m] = sum;
    }
    B_max = n;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;              /* a(0) = 1 */

    /* need B[n] and B[n+1] */
    ensure_B(n + 1);

    /* a(n) = (B[n+1] - B[n]) / 2 */
    seq_t diff = B[n + 1] - B[n];
    return diff / (seq_t)2;
}
