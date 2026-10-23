/*
 * A170640  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A170640: Number of reduced words of length n in the Coxeter
 *  group on 7 generators with relations S_i^2 = (S_i S_j)^49 = I.
 *
 *  The sequence satisfies the linear recurrence
 *
 *       a(0) = 1
 *       a(n) = 5 * Σ_{k=1}^{n} a(n-k) + 2          for 1 ≤ n ≤ 48
 *       a(49) = 5 * Σ_{k=1}^{48} a(k) + 1 – 15*a(0)
 *       a(n) = 5 * Σ_{i=n-48}^{n-1} a(i) – 15*a(n-49)   for n ≥ 50
 *  (Σ over an empty set is 0).  This follows from the rational
 *  generating function displayed in the OEIS entry.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return 1;
    }

    /* Allocate array for the first n+1 terms.  For the typical
     * integer builds this is trivial; for arbitrary precision
     * builds the type seq_t is assumed to be trivially
     * constructible when stored in malloced memory. */
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr) {
        return 0;                 /* allocation failure – unlikely in tests */
    }

    /* base term */
    arr[0] = 1;

    /* ----- 1 ≤ i ≤ 48 : inhomogeneous recurrence ---- */
    seq_t cumulative = arr[0];  /* Σ_{j=0}^{i-1} a(j)  */

    if (n <= 48) {
        for (idx_t i = 1; i <= n; ++i) {
            arr[i] = (seq_t)5 * cumulative + 2;    /* 5*sum + p_i (p_i = 2) */
            cumulative += arr[i];
        }
        seq_t res = arr[n];
        free(arr);
        return res;
    }

    /* i = 49 : special coefficient from numerator */
    for (idx_t i = 1; i <= 48; ++i) {
        arr[i] = (seq_t)5 * cumulative + 2;
        cumulative += arr[i];
    }

    /* Σ_{k=1}^{48} a(k) */
    seq_t sum_1_to_48 = cumulative - arr[0];
    arr[49] = (seq_t)5 * sum_1_to_48 + 1 - (seq_t)15 * arr[0];

    if (n == 49) {
        seq_t res = arr[49];
        free(arr);
        return res;
    }

    /* ----- n ≥ 50 : homogeneous recurrence with sliding window ---- */
    /* Initialize sliding window sum of the last 48 terms: a[1]..a[48] */
    seq_t win_sum = sum_1_to_48;

    for (idx_t i = 50; i <= n; ++i) {
        idx_t idx_sub = i - 49;          /* a(i-49) for the "-15*" term */
        seq_t term = (seq_t)5 * win_sum - (seq_t)15 * arr[idx_sub];
        arr[i] = term;

        /* update sliding window: new sum covers a(i-47)..a(i) */
        win_sum += arr[i] - arr[i - 48];
    }

    seq_t res = arr[n];
    free(arr);
    return res;
}
