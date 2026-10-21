/*
 * A074747  Largest difference between consecutive anti-divisors of n (ordered
 *          by size).
 * https://oeis.org/A074747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code implements the definition of sequence A074747.
   For a given n, it enumerates all integers k with 2 ≤ k < n such that
   - if k is even,   n ≡ k/2 (mod k);
   - if k is odd,    n ≡ (k-1)/2 or (k+1)/2 (mod k).
   The sequence a(n) is the maximum difference between two consecutive
   anti‑divisors of n; it is 0 when fewer than two anti‑divisors exist. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t max_diff = 0;   /* largest difference found */
    seq_t prev_diff = 0;  /* previous anti‑divisor (as seq_t) */
    bool first = true;    /* true until the first anti‑divisor is found */

    /* Scan all candidates k = 2 … n‑1 */
    for (idx_t k = 2; k < n; ++k) {
        idx_t r = n % k;          /* remainder of n divided by k      */
        bool is_anti = false;

        if ((k & 1ULL) == 0) {    /* k even */
            if (r == k / 2) {
                is_anti = true;
            }
        } else {                  /* k odd */
            idx_t a = (k - 1) / 2;
            idx_t b = (k + 1) / 2;
            if (r == a || r == b) {
                is_anti = true;
            }
        }

        if (is_anti) {
            seq_t cur = (seq_t)k;
            if (first) {
                /* first anti‑divisor found */
                first = false;
            } else {
                seq_t diff = cur - prev_diff;
                if (diff > max_diff) {
                    max_diff = diff;
                }
            }
            prev_diff = cur;
        }
    }
    return max_diff;
}
