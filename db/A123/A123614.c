/*
 * A123614  Column 4 of triangle A123610.
 * https://oeis.org/A123614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p[n] - Σ_{j=1}^{min(20,n)} d[j] * a[n-j], where d[j] are
 *            the coefficients of D(x)=∏_{k=1}^4(1−x^k)^2 and p[n] are the
 *            coefficients of P_4(x) (zero for n≥16). This recurrence is
 *            implemented iteratively with a growable cache
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 56, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* realloc, abort */

/* Coefficients of the denominator polynomial D(x) = (1‑x)^2(1‑x²)^2(1‑x³)^2(1‑x⁴)^2.
   d[0] is 1; d[1]..d[20] are the remaining coefficients. */
static const int d[21] = {
    1,  /* d[0] */
   -2, -1,  2,  1,  4, -4, -4, -2,  0,
   10,  0, -2, -4, -4,  4,  1,  2, -1, -2,  1
};

/* Coefficients of the numerator polynomial P_4(x). */
static const int p[16] = {
    1, 3, 28, 94, 240, 440, 679, 839,
    887, 757, 550, 314, 148,  48,  11,  1
};

/* Cached values of a(n).  The array grows on demand. */
static seq_t *a_arr = NULL;
static idx_t  a_len  = 0;

/* Extend the cache so that a_arr contains a(0) .. a(N). */
static void extend_to(idx_t N)
{
    /* Allocate or reallocate storage. */
    seq_t *tmp = (seq_t *)realloc(a_arr, (N + 1) * sizeof(seq_t));
    if (!tmp) abort();      /* allocation failed */
    a_arr = tmp;

    /* Compute missing terms. */
    for (idx_t i = a_len; i <= N; ++i) {
        if (i == 0) {          /* a(0) = 1 */
            a_arr[0] = (seq_t)1;
            continue;
        }

        /* Accumulate the sum of d[j] * a[i-j] for j = 1 .. min(20,i). */
        seq_t pos = (seq_t)0;   /* sum of positive‑coefficient contributions */
        seq_t neg = (seq_t)0;   /* sum of absolute values of negative‑coefficient contributions */

        int maxj = (i < 20) ? (int)i : 20;
        for (int j = 1; j <= maxj; ++j) {
            const int coeff = d[j];
            if (coeff == 0) continue;

            seq_t prev = a_arr[i - (idx_t)j];
            int abscoeff = (coeff > 0) ? coeff : -coeff;

            /* Add abscoeff copies of prev to the appropriate accumulator. */
            for (int k = 0; k < abscoeff; ++k) {
                if (coeff > 0)
                    pos += prev;
                else
                    neg += prev;
            }
        }

        /* Right‑hand side from the numerator (zero if i ≥ 16). */
        seq_t rhs = (i < 16) ? (seq_t)p[(int)i] : (seq_t)0;

        /* a[i] = rhs + neg - pos  (since d[0] = 1). */
        seq_t val = rhs;
        val += neg;
        val -= pos;
        a_arr[i] = val;
    }

    a_len = N + 1;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n >= a_len)
        extend_to(n);
    return a_arr[n];
}
