/*
 * A182185  G.f.: exp( Sum_{n>=1} 3^b(n) * x^n/n ) where b(n) = highest
 *          exponent of 3 in 2^n+1.
 * https://oeis.org/A182185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the functional equation A(x) =
 *            ((1-x^2)*(1-x^3)/(1-x)^3) * A(x^3). Recurrence derived from
 *            this: for n ≥ 1 a(n) = Σ_{m=0}^{⌊n/3⌋} c_{n-3m} · a(m) , where
 *            c_k = 1 (k=0), 3 (k=1), 5 (k=2), and 6 (k≥3)
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoizes previously computed terms and
   computes new terms on demand.  */

#include "../utils/seq_realloc.h"

static seq_t *seq = NULL;      /* memoization storage */
static size_t seq_len = 0;     /* number of elements currently stored */

/* Ensure that seq[n] is computed for all n ≤ target */
static void ensure_seqs(idx_t target)
{
    if (target < seq_len)
        return;                       /* already computed */

    /* Allocate or resize storage */
    size_t new_len = (size_t)target + 1;
    seq = seq_realloc(seq, seq_len, new_len);
    if (seq_len == 0)                  /* first allocation */
    {
        seq[0] = (seq_t)1;             /* a(0) = 1 */
        seq_len = 1;
    }

    /* Compute new terms */
    for (idx_t i = seq_len; i <= target; ++i)
    {
        seq_t sum = 0;
        idx_t max_m = i / 3;
        for (idx_t m = 0; m <= max_m; ++m)
        {
            idx_t k   = i - 3 * m;
            seq_t ck;
            if (k == 0)
                ck = (seq_t)1;
            else if (k == 1)
                ck = (seq_t)3;
            else if (k == 2)
                ck = (seq_t)5;
            else
                ck = (seq_t)6;

            sum += ck * seq[(size_t)m];
        }
        seq[(size_t)i] = sum;
    }
    seq_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_seqs(n);
    return seq[(size_t)n];
}
