/*
 * A238781  Number of palindromic partitions of n whose least part has
 *          multiplicity 1.
 * https://oeis.org/A238781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Σ_{c=1}^{n-1, n ≡ c (mod 2)} p_{>c}((n-c)/2) where
 *            p_{>c}(k) is the number of partitions of k into parts strictly
 *            greater than c (in the OEIS sense of palindromic partitions)
 * Range:     u8 n <= 46, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function below computes the OEIS sequence A238781.
    It uses only integer arithmetic, adheres to the
    strict type requirements and is fully C11/C++17 compatible. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static storage for memoised results up to the largest n seen so far. */
    static idx_t max_n = 0;              /* largest n that has been computed */
    static seq_t *a_arr = NULL;          /* a_arr[1..max_n] */

    if (n < 1) return 0;                 /* undefined for n < 1 */

    if (n > max_n) {
        /* Allocate a new array big enough for the extended range. */
        size_t new_size = (size_t)(n + 1);        /* indices 0..n, 0 unused */
        seq_t *new_a = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!new_a) return 0;                  /* allocation failed */

        /* Initialise all positions with the trivial partition of length 1. */
        for (size_t i = 1; i <= n; ++i)
            new_a[i] = 1;

        /* Dynamic programming for partitions with parts strictly greater
           than a given threshold.  We compute contributions for all n
           in one sweep from the largest applicable threshold down to 2.
           (Threshold 1 would correspond to a smallest part of 0, which is
           impossible, so we stop at 2.) */
        size_t Smax = (size_t)(n / 2);          /* maximal value of (n-c)/2 */

        /* p[s] will hold the number of partitions of s into parts
           >= current threshold.  Initially all parts are forbidden
           (threshold Smax+1) so only the empty partition exists. */
        seq_t *p = (seq_t*)calloc(Smax + 1, sizeof(seq_t));
        if (!p) {
            free(new_a);
            return 0;
        }
        p[0] = 1;                               /* partitions of 0 */

        for (size_t t = Smax; t >= 2; --t) {    /* threshold t = c+1, so c=t-1 */
            /* Update 'p' to include parts of size exactly 't'. */
            for (size_t s = t; s <= Smax; ++s)
                p[s] += p[s - t];

            /* Now add the contribution of this threshold to every n
             * that can be formed with it.  For a given n we need
             * s = (n - (t-1)) / 2 and it must satisfy
             *          n ≡ t-1 (mod 2)   (parity)
             *          s ≥ t            (each other part > c ≥ t-1),
             * which in turn is equivalent to n ≥ 3*t-1. */
            size_t start_n = (3 * t - 1 > t) ? (3 * t - 1) : t;
            for (size_t nn = start_n; nn <= n; nn += 2) {
                size_t s_val = (nn - (t - 1)) / 2;   /* integer division */
                new_a[nn] += p[s_val];
            }
        }
        free(p);

        /* Replace the old memoised array (if any). */
        if (a_arr) free(a_arr);
        a_arr = new_a;
        max_n = n;
    }

    return a_arr[n];
}
