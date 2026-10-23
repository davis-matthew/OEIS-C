/*
 * A170104  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A170104 – number of reduced words of length n in a 47‑generator Coxeter
    group with relations (S_i)^2 = (S_i S_j)^37 = I.
    Recurrence (derived from the rational generating function):
        a(0) = 1
        a(n) = 2 + 45 * Σ_{k=0}^{n-1} a(k)          for 1 ≤ n ≤ 36
        a(n) = 45 * Σ_{k=n-36}^{n-1} a(k) − 1035 * a(n-37)   for n ≥ 37
    The function computes the term a(n) by iterating from 0 up to n
    (O(n) time, O(n) space for a temporary array).  It works for any
    seq_t / idx_t type supplied by the build system.  No global state is
    used, so calls in any order produce the same result. */

#include <stdlib.h>

/* External symbol declarations – provided by the build system
 *
 *   typedef ... idx_t;   // e.g. uint64_t
 *   typedef ... seq_t;   // e.g. unsigned char, uint64_t, unsigned __int128, or big integer class
 *   seq_t a(idx_t n);
 */

seq_t a(idx_t n)
{
    /* Handle offset 0 directly. */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Allocate array for terms 0..n. */
    seq_t *const vals = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!vals) /* allocation failure: return 0 – unlikely in the test harness */
        return (seq_t)0;

    vals[0] = (seq_t)1;
    seq_t prefix_sum = vals[0];   /* Σ_{k=0}^{i-1} a(k) for the current i */

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        if (i <= (idx_t)36) {
            /* a(i) = 2 + 45 * Σ_{k=0}^{i-1} a(k) */
            vals[i] = (seq_t)2 + (seq_t)45 * prefix_sum;
            prefix_sum += vals[i];
        } else {
            /* a(i) = 45 * Σ_{k=i-36}^{i-1} a(k) – 1035 * a(i-37) */
            seq_t sum_last36 = (seq_t)0;
            const idx_t start = i - (idx_t)36;
            for (idx_t j = start; j < i; ++j)
                sum_last36 += vals[j];
            vals[i] = (seq_t)45 * sum_last36 - (seq_t)1035 * vals[i - (idx_t)37];
        }
    }

    const seq_t result = vals[n];
    free(vals);
    return result;
}
