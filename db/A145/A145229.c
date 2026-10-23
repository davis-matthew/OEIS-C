/*
 * A145229  Coefficients in expansion of E'_1(q).
 * https://oeis.org/A145229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * (n+1) * d(n+1) * where d(m) is the number of positive
 *            divisors of m. * This matches A145229, which equals 4 times
 *            A038040 * (the divisor-count sequence shifted by one)
 * Range:     u8 n <= 10, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *divcnt = NULL;   /* divisor counts for 0..N */
static size_t divcnt_size = 0; /* current size of divcnt array */

/* Ensure that divisor counts are available up to "upto" (inclusive). */
static void ensure_divcnt(size_t upto)
{
    if (upto + 1 <= divcnt_size)
        return;                               /* already computed */

    /* Allocate new array and compute divisor counts from scratch. */
    size_t newsize = upto + 1;
    seq_t *newarr = (seq_t *)malloc((newsize + 1) * sizeof(seq_t));
    if (!newarr)                         /* allocation failed – leaf program */
        return;

    for (size_t i = 0; i <= newsize; ++i)
        newarr[i] = 0;                   /* init to zero */

    for (size_t i = 1; i <= newsize; ++i)
        for (size_t j = i; j <= newsize; j += i)
            ++newarr[j];                 /* count divisor i of j */

    free(divcnt);
    divcnt     = newarr;
    divcnt_size = newsize;
}

seq_t a(idx_t n)
{
    /* We need d(n+1), so ensure divisor counts up to (n+1). */
    ensure_divcnt((size_t)n + 1);

    seq_t d = divcnt[(size_t)n + 1];          /* d(n+1) */
    seq_t result = 4 * ((seq_t)(n + 1)) * d; /* 4*(n+1)*d(n+1) */
    return result;
}
