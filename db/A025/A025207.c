/*
 * A025207  a(n) = [ Sum{(log(j)-log(i))^3} ], 2 <= i < j <= n.
 * https://oeis.org/A025207
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊ Σ_{2≤i<j≤n} (log(j)-log(i))^3 ⌋ , offset 3
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (log is the natural logarithm, result is the nearest integer less than or equal to the sum) */

#include <stdlib.h>
#include <math.h>

static long double *logv = NULL; /* logv[k] = log(k) for k ≥ 2 */
static idx_t cached_n = 0;

static void ensure_logs(idx_t n)
{
    if (n <= cached_n)
        return;
    size_t new_size = (size_t)(n + 1);
    long double *tmp = (long double *)realloc(logv, new_size * sizeof(long double));
    if (!tmp)
        abort();                     /* allocation failure; program fails safely */
    logv = tmp;
    for (idx_t i = cached_n + 1; i <= n; ++i)
        logv[i] = logl((long double)i);
    cached_n = n;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;              /* undefined for OEIS offset */

    ensure_logs(n);

    /* Initial sums for indices i = 2 only */
    long double sum_log  = logv[2];
    long double sum_log2 = sum_log * sum_log;
    long double sum_log3 = sum_log2 * sum_log;

    long double total = 0.0L;

    for (idx_t j = 3; j <= n; ++j) {
        long double l  = logv[j];
        long double l2 = l * l;
        long double l3 = l2 * l;

        long double term = (long double)(j - 2) * l3
                           - 3.0L * l2 * sum_log
                           + 3.0L * l * sum_log2
                           - sum_log3;

        total += term;

        /* Update cumulative sums to include index j */
        sum_log += l;
        sum_log2 += l2;
        sum_log3 += l3;
    }

    /* Return the floor of the sum; add a tiny epsilon to guard against
       floating‑point errors that might push the result just above an
       integer value. */
    return (seq_t)((unsigned long long)floorl(total + 1e-12L));
}
