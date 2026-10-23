/*
 * A162888  An alternating sum of the first n nonprimes.
 * https://oeis.org/A162888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A064455(A141468(n)+1) where A141468(n) is the n-th
 *            nonprime with 0 as the first nonprime. Let x = A141468(n). Then
 *            a(n) = Σ_{k=0}^{x} (1 - (-1)^k * k). The sum simplifies to: if x
 *            is even (x = 2m) a(n) = m + 1 if x is odd (x = 2m+1) a(n) = 3 *
 *            (m + 1)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/* Test whether n is prime (trial division, adequate for n <= ~2e4) */
static bool is_prime_uint64(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t d = 3; d * d <= n; d += 2)
        if (n % d == 0)
            return false;
    return true;
}

/* Return the n-th nonprime (0‑based: 1 → 0, 2 → 1, 3 → 4, …) */
static idx_t nth_nonprime_idx(idx_t n)
{
    idx_t cnt = 0;
    idx_t num = 0;
    while (true) {
        if (!is_prime_uint64(num)) {
            cnt++;
            if (cnt == n)
                return num;
        }
        num++;
    }
}

/* Main function required by the build */
seq_t a(idx_t n)
{
    /* Find the n-th nonprime (with 0 as the first) */
    idx_t x = nth_nonprime_idx(n);

    /* Compute the alternating sum S(x) */
    if ((x & 1ULL) == 0) { /* x even: x = 2m  */
        seq_t m = (seq_t)(x / 2);
        return m + 1;
    } else {               /* x odd:  x = 2m+1 */
        seq_t m = (seq_t)((x - 1) / 2);
        return ((seq_t)3) * (m + 1);
    }
}
