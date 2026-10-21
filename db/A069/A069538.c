/*
 * A069538  Duplicate of A052217.
 * https://oeis.org/A069538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n-th positive integer with decimal digit sum 3, starting
 *            with a(0) = 3
 * Range:     i8 n <= 6, i16 n <= 16, i32 n <= 16, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Binomial coefficient C(n,k) for small n (n<=70). */
static idx_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;              /* use symmetry */
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;       /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the length L of the n-th number.
       C(L+2,3) = total count of numbers with length <= L.
       Find smallest L with C(L+2,3) > n. */
    idx_t L = 1;
    while (binom(L + 2, 3) <= n) ++L;

    /* Offset within numbers of length L (0‑based) */
    idx_t k = n - binom(L + 1, 3);

    /* Unrank the number of length L with sum of digits 3. */
    seq_t result = 0;
    idx_t rem_sum = 3;                    /* remaining digit sum to distribute */

    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t start_digit = (pos == 0) ? 1 : 0;
        idx_t R = L - pos - 1;            /* remaining positions after this one */

        for (idx_t d = start_digit; d <= 3; ++d) {
            if (d > rem_sum) break;
            idx_t rem_after = rem_sum - d;
            idx_t ways;
            if (R == 0) {
                ways = (rem_after == 0) ? 1 : 0;
            } else {
                ways = binom(rem_after + R - 1, R - 1);
            }

            if (k < ways) {
                /* choose digit d */
                result = result * 10 + (seq_t)d;
                rem_sum = rem_after;
                break;
            } else {
                k -= ways;
            }
        }
    }
    return result;
}
