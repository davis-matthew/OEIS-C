/*
 * A074885  Numbers not of the form x^2 + M*y^2 for integers x > 0, y > 1, M >
 *          0.
 * https://oeis.org/A074885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n-th positive integer that cannot be expressed as x^2 +
 *            M*y^2 for integers x > 0, y > 1, M > 0. The sequence is finite;
 *            all terms ≤ 1875902 have been precomputed by a sieve
 * Range:     u8 n <= 38, u16 n <= 343, u32 n <= 436, u64 n <= 436,
 *            u128 n <= 436, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static const unsigned int LIMIT = 1875902;   /* empirical maximum value */
static bool *representable = NULL;          /* bitmap of representable numbers */
static seq_t *seq_vals = NULL;              /* array of sequence values */
static idx_t seq_len = 0;                   /* number of terms in the sequence */

/* initialise the sieve once */
static void init_sequence(void)
{
    if (representable != NULL) return;          /* already initialised */

    /* allocate bitmap */
    representable = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!representable) return;                 /* allocation failure */

    /* mark all numbers representable as x^2 + M*y^2 */
    for (int x = 1; (long long)x * x <= LIMIT; ++x)
    {
        long long xsq = (long long)x * x;
        for (int y = 2; ; ++y)
        {
            long long ysq = (long long)y * y;
            if (ysq > (long long)LIMIT - xsq) break;
            for (long long n = xsq + ysq; n <= LIMIT; n += ysq)
                representable[(size_t)n] = true;
        }
    }

    /* count how many numbers are NOT representable */
    seq_len = 0;
    for (size_t i = 1; i <= LIMIT; ++i)
        if (!representable[i]) ++seq_len;

    /* store the sequence values */
    seq_vals = (seq_t *)malloc(seq_len * sizeof(seq_t));
    if (!seq_vals) return; /* allocation failure */
    idx_t idx = 0;
    for (size_t i = 1; i <= LIMIT; ++i)
        if (!representable[i]) seq_vals[idx++] = (seq_t)i;
}

/* public function required by the build */
seq_t a(idx_t n)
{
    init_sequence();
    if (n == 0 || n > seq_len) return (seq_t)0;
    return seq_vals[n - 1];
}
