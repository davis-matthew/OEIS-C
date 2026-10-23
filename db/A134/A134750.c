/*
 * A134750  Variant of permutational numbers with shifted digits
 *          0->1->2->...->k+1 in k+1 positional system.
 * https://oeis.org/A134750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth smallest integer obtained by * interpreting a
 *            k‑digit permutation of {1,…,k} in base k * (k is chosen so that
 *            the cumulative count of terms up to * base k reaches n)
 * Range:     u8 n <= 25, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;             /* base k = 1, term 1 */

    /* Find the base k such that 1 + sum_{j=2}^{k} j!  >= n.  */
    seq_t target = (seq_t)n;                 /* use seq_t for safety */
    seq_t cum = 1;                           /* S(1) = 1 */
    seq_t fact = 1;                          /* current factorial j! */

    idx_t k = 1;
    for (idx_t j = 2;; ++j) {
        fact = fact * (seq_t)j;              /* j! */
        if (cum + fact >= target) {
            k = j;
            break;
        }
        cum += fact;
    }

    /* Rank of the wanted permutation within the block for base k. */
    seq_t r = target - cum - 1;              /* 0‑based */

    /* Store factorials up to k in an array for quick access. */
    seq_t *fact_arr = (seq_t *)malloc((k + 1) * sizeof(seq_t));
    fact_arr[0] = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        fact_arr[i] = fact_arr[i - 1] * (seq_t)i;
    }

    /* Build the list of available digits 1..k. */
    idx_t *digits = (idx_t *)malloc(k * sizeof(idx_t));
    for (idx_t i = 0; i < k; ++i) digits[i] = i + 1;

    seq_t value = 0;
    idx_t remaining = k;
    for (idx_t pos = k; pos >= 1; --pos) {
        seq_t f = fact_arr[pos - 1];
        idx_t index = (idx_t)(r / f);       /* index of next digit */
        r = r % f;

        seq_t d = (seq_t)digits[index];
        value = value * (seq_t)k + d;        /* accumulate base‑k value */

        /* Remove used digit from the array. */
        if (index + 1 < remaining)
            memmove(&digits[index], &digits[index + 1],
                    (remaining - index - 1) * sizeof(idx_t));
        --remaining;
    }

    free(digits);
    free(fact_arr);
    return value;
}
