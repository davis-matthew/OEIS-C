/*
 * A026053  (d(n)-r(n))/5, where d = A008778 and r is the periodic sequence
 *          with fundamental period (0,3,1,0,1).
 * https://oeis.org/A026053
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-5) - 3*a(n-6)
 *             + 3*a(n-7) - a(n-8)   for n ≥ 10, offset = 2.
 * Initial values (offset 2):
 *   a(2)=1, a(3)=2, a(4)=5, a(5)=9, a(6)=14, a(7)=21, a(8)=29, a(9)=40
 */

#if defined(__cplusplus)
# include <cstdlib>
#else
# include <stdlib.h>
#endif

/* static table of the first ten values (indices 0..9) */
static const seq_t _base[10] = {
    0, 0,                   /* a(0), a(1) – unused but safe */
    1, 2, 5, 9, 14, 21, 29, /* a(2) … a(8) */
    40                     /* a(9) */
};

seq_t a(idx_t n)
{
    /* values below the offset are irrelevant; return 0 */
    if (n < 2)
        return 0;

    /* For n <= 9 we can answer directly from the static table. */
    if (n <= 9)
        return _base[n];

    /* Allocate a temporary array large enough to hold a(0)…a(n). */
#if defined(__cplusplus)
    seq_t *seq = new seq_t[n + 1];
#else
    seq_t *seq = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!seq)
        abort();
#endif

    /* Initialise the known values. */
    for (idx_t i = 0; i <= 9; ++i)
        seq[i] = _base[i];

    /* Compute the remaining terms using the recurrence. */
    for (idx_t i = 10; i <= n; ++i) {
        seq[i] = 3 * seq[i - 1]    /* 3*a(i-1) */
              - 3 * seq[i - 2]    /* -3*a(i-2) */
              + seq[i - 3]        /* +a(i-3) */
              + seq[i - 5]        /* +a(i-5) */
              - 3 * seq[i - 6]    /* -3*a(i-6) */
              + 3 * seq[i - 7]    /* +3*a(i-7) */
              - seq[i - 8];       /* -a(i-8) */
    }

    seq_t result = seq[n];

#ifdef __cplusplus
    delete[] seq;
#else
    free(seq);
#endif

    return result;
}
