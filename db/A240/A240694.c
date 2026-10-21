/*
 * A240694  Partial products of divisors of n, cf. A027750.
 * https://oeis.org/A240694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 83, u32 n <= 260, u64 n <= 970,
 *            u128 n <= 4860, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th element of the triangle where each row m
   lists the partial products T(m,k)=∏_{i=1..k} d_i,
   d_i being the i‑th smallest divisor of m.
   The sequence is read row by row.
   Formula: a(n) = T(m,k) where
     m = smallest integer with Σ_{i=1..m} d(i) ≥ n
     k = n - Σ_{i=1..m-1} d(i)   (1‑based index within the row) */
#include <stdint.h>
#include <stddef.h>

#define MAX_DIV 2048

/* Fill sorted array divs[] with all positive divisors of m
   (in ascending order). The number of divisors is returned in *cnt. */
static void get_divisors(uint64_t m, uint32_t divs[], size_t *cnt)
{
    uint32_t low[MAX_DIV];
    uint32_t high[MAX_DIV];
    size_t lowc = 0, highc = 0;

    for (uint64_t d = 1; d <= m / d; ++d) {
        if (m % d == 0) {
            if (lowc < MAX_DIV) low[lowc++] = (uint32_t)d;
            uint64_t d2 = m / d;
            if (d2 != d && highc < MAX_DIV) high[highc++] = (uint32_t)d2;
        }
    }

    size_t k = 0;
    for (size_t i = 0; i < lowc; ++i) divs[k++] = low[i];
    for (size_t i = highc; i > 0; --i) divs[k++] = high[i - 1];

    *cnt = k;
}

seq_t a(idx_t N)
{
    if (N == 0) return (seq_t)0;  /* offset starts at 1, but guard */

    idx_t total = 0;               /* number of entries emitted so far */
    uint32_t divs[MAX_DIV];
    size_t dcnt;

    for (idx_t m = 1; ; ++m) {
        get_divisors((uint64_t)m, divs, &dcnt);

        /* Does this row reach index N? */
        if (total + (idx_t)dcnt >= N) {
            size_t pos = (size_t)(N - total - 1);   /* 0‑based position in row */
            seq_t prod = (seq_t)1;
            for (size_t i = 0; i <= pos; ++i)
                prod *= (seq_t)divs[i];
            return prod;
        }
        total += (idx_t)dcnt;
    }
}
