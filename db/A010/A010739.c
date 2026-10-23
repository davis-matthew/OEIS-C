/*
 * A010739  Shifts 2 places left under inverse binomial transform.
 * https://oeis.org/A010739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, eigen
 * Formula:   a(0)=1, a(1)=2, a(2)=1, a(3)=1. For n >= 2: a(n) =
 *            sum_{k=0}^{n-2} (-1)^k * C(n-2,k) * a(n-2-k). This follows from
 *            the generating‑function relation A(x)=1+2x+x²*A(x/(1+x))/(1+x)
 * Range:     i8 n <= 8, i16 n <= 12, i32 n <= 19, i64 n <= 31, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_cache      = NULL;   /* array of previously computed a(n) */
static size_t seq_cap       = 0;      /* current allocated capacity */
static idx_t   seq_last     = 0;      /* largest n that has been computed */

static void ensure_cache(idx_t n)
{
    /* initial allocation with base values */
    if (!seq_cache) {
        seq_cap = 8;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_cache) abort();

        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)2;
        seq_cache[2] = (seq_t)1;
        seq_cache[3] = (seq_t)1;
        seq_last = 3;
    }

    if (n <= seq_last) return;

    /* grow array if necessary */
    if ((size_t)n >= seq_cap) {
        size_t new_cap = seq_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        seq_cap   = new_cap;
    }

    /* compute new terms from seq_last+1 up to n */
    for (idx_t i = seq_last + 1; i <= n; ++i) {
        idx_t m   = i - 2;                 /* upper bound of binomial index */
        seq_t  bin = (seq_t)1;              /* C(m,0) */
        seq_t  sum = (seq_t)0;

        for (idx_t k = 0; k <= m; ++k) {
            seq_t term = bin * seq_cache[i - k - 2];
            if (k & 1) term = -term;       /* apply (-1)^k */
            sum += term;

            if (k < m) {
                seq_t numerator   = (seq_t)(m - k);
                seq_t denominator = (seq_t)(k + 1);
                bin = (bin * numerator) / denominator;
            }
        }
        seq_cache[i] = sum;
    }
    seq_last = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
