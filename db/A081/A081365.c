/*
 * A081365  Numbers whose name in American English is a word-palindrome,
 *          reading the same forward and backward.
 * https://oeis.org/A081365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   for 1 ≤ n ≤ 9, a(n) = n × 100 + n for n > 9: let m = n – 10, i =
 *            ⌊m / 99⌋ + 1, k = (m mod 99) + 1 then a(n) = k × 10^(3 i) + k
 * Range:     u8 n <= 2, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint8_t, etc., but seq_t is supplied by the build */

seq_t a(idx_t n)
{
    /* Numbers are defined only for n ≥ 1 */
    if (n == 0) return 0;

    /* The first block: 101, 202, …, 909 */
    if (n <= 9) {
        seq_t k = (seq_t)n;
        return k * (seq_t)100 + k;
    }

    /* For n > 9 the pattern is k × 10^(3*i) + k,
       with i ≥ 1, 1 ≤ k ≤ 99. */
    idx_t m = n - 10;                  /* zero‑based position after first block */
    idx_t i = m / 99 + 1;              /* block index (k repeats every 99) */
    idx_t k_idx = m % 99 + 1;           /* k value within the block */

    /* Compute 10^(3*i) incrementally.
       Using a loop keeps the code simple and avoids dynamic memory. */
    seq_t pow = (seq_t)1;              /* 10^(3*0) = 1 */
    for (idx_t j = 0; j < i; ++j) {
        pow = pow * (seq_t)1000;        /* multiply by 10^3 each step */
    }

    seq_t k = (seq_t)k_idx;
    return pow * k + k;                 /* k × 10^(3*i) + k */
}
