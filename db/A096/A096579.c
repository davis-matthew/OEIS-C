/*
 * A096579  Number of partitions of an n-set with exactly one even block.
 * https://oeis.org/A096579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of partitions of an n‑set with exactly one even
 *            block Recurrence (derived from the e.g.f.
 *            exp(sinh(x))*(cosh(x)-1)): b(0) = 1 b(n) = Σ_{s odd, 1≤s≤n}
 *            C(n‑1, s‑1) * b(n‑s) (partitions into odd blocks) a(0) = 0 a(n)
 *            = Σ_{s odd, 1≤s≤n} C(n‑1, s‑1) * a(n‑s) + Σ_{s even, 2≤s≤n}
 *            C(n‑1, s‑...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp_b = NULL;  /* b values */
static seq_t *dp_a = NULL;  /* a values */
static idx_t  dp_cap = 0;   /* allocated array size */
static idx_t  dp_computed = 0; /* largest n for which values are known */

/* Grow the tables to at least indices 0 … n */
static void ensure_tables(idx_t n)
{
    if (n + 1 <= dp_cap) return;

    size_t new_cap = dp_cap ? dp_cap << 1 : 1;
    while (new_cap <= n) new_cap <<= 1;

    seq_t *tmp_b = (seq_t *)realloc(dp_b, new_cap * sizeof(seq_t));
    seq_t *tmp_a = (seq_t *)realloc(dp_a, new_cap * sizeof(seq_t));
    if (!tmp_b || !tmp_a) {
        /* Allocation failed – terminate with diagnostic */
        abort();
    }
    dp_b = tmp_b;
    dp_a = tmp_a;
    dp_cap = new_cap;

    if (dp_computed == 0) {
        dp_b[0] = (seq_t)1;
        dp_a[0] = (seq_t)0;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset is 1 */

    ensure_tables(n);

    if (dp_computed < n) {
        for (idx_t m = dp_computed + 1; m <= n; ++m) {
            seq_t sum_b = (seq_t)0;
            seq_t sum_a = (seq_t)0;
            seq_t bin = (seq_t)1;          /* C(m‑1,0) */

            /* iterate over block sizes s = 1 … m */
            for (idx_t s = 1; s <= m; ++s) {
                /* bin now equals C(m‑1, s‑1) */
                if (s & 1) {               /* odd block size */
                    sum_b += bin * dp_b[m - s];
                    sum_a += bin * dp_a[m - s];
                } else {                   /* even block size */
                    sum_a += bin * dp_b[m - s];
                }

                if (s < m) {
                    /* update bin to C(m‑1, s) for the next iteration */
                    bin = bin * (seq_t)(m - s) / (seq_t)s;
                }
            }

            dp_b[m] = sum_b;
            dp_a[m] = sum_a;
        }
        dp_computed = n;
    }

    return dp_a[n];
}
