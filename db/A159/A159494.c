/*
 * A159494  Numerator of Hermite(n, 3/13).
 * https://oeis.org/A159494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 13^n * Hermite(n, 3/13) Recurrence: a(0)=1, a(1)=6 a(n) =
 *            6*a(n-1) - 338*(n-1)*a(n-2) for n>=2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A159494. */
seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* a(n-2) and a(n-1) values during the iteration. */
    seq_t prev = (seq_t)6;      /* a(1) */
    seq_t curr = (seq_t)1;      /* a(0) */

    /* Iteratively compute a(2) ... a(n). */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t temp   = (seq_t)6 * prev;                               /* 6*a(n-1) */
        seq_t coeff  = (seq_t)338 * (i - 1);                           /* 338*(n-1) */
        seq_t prod   = coeff * curr;                                  /* 338*(n-1)*a(n-2) */
        seq_t next   = temp - prod;                                   /* a(n) */

        curr = prev;  /* shift: a(n-2) becomes a(n-1) for next step */
        prev = next;  /* a(n-1) becomes a(n) for next step */
    }

    return prev;   /* 'prev' now holds a(n) */
}
