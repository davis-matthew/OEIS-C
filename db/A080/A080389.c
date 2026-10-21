/*
 * A080389  Numbers n such that gcd( n+1, binomial(n, floor(n/2)) ) = 1.
 * https://oeis.org/A080389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) is the nth positive integer m such that gcd(m+1,
 *            binomial(m, floor(m/2))) = 1. The function generates the numbers
 *            in increasing order and tests each candidate
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   is_binomial_divisible_by_p
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_binomial_divisible_by_p.h"

/* Storage for the sequence – enough for all requested n up to about 10 000. */
static seq_t good[20001];     /* 1‑based in the sense that good[0] will hold a(1) */
static size_t good_len = 0;   /* number of terms currently stored */
static idx_t processed_n = 0; /* largest integer that has been examined */

/* Return 1 iff m satisfies the definition of the sequence. */
static int is_good(idx_t m)
{
    idx_t k = m >> 1;   /* floor(m/2) */
    idx_t temp = m + 1;

    /* check factor 2 separately */
    if ((temp & 1) == 0) {
        if (is_binomial_divisible_by_p(m, k, 2)) return 0;
        while ((temp & 1) == 0) temp >>= 1;
    }

    /* check odd prime factors of m+1 */
    for (idx_t d = 3; d <= temp / d; d += 2) {
        if (temp % d == 0) {
            if (is_binomial_divisible_by_p(m, k, d)) return 0;
            while (temp % d == 0) temp /= d;
        }
    }
    /* any remaining factor > sqrt(temp) is a prime */
    if (temp > 1 && is_binomial_divisible_by_p(m, k, temp))
        return 0;

    return 1;
}

/* External function requested by the build. */
seq_t a(idx_t n)
{
    while (good_len < n) {
        ++processed_n;
        if (is_good(processed_n)) {
            good[good_len++] = (seq_t)processed_n;
        }
    }
    return good[n - 1];
}
