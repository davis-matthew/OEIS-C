/*
 * A144551  a(n) = nonprime(n)*nonprime(n+1)/2, where nonprime(n) =
 *          A141468(n).
 * https://oeis.org/A144551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nonprime(n)*nonprime(n+1)/2 where nonprime(k) is the k‑th
 *            nonprime (0,1,4,6,8,9,10,…)
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime_trial(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Find the n‑th and (n+1)‑th nonprime numbers */
    seq_t nonprime_n = 0, nonprime_np1 = 0;
    idx_t count = 0;
    for (idx_t x = 0; count < n + 1; ++x) {
        if (!is_prime_trial(x)) {  /* x is nonprime */
            ++count;
            if (count == n)     nonprime_n  = (seq_t)x;
            if (count == n + 1) nonprime_np1 = (seq_t)x;
        }
    }

    /* Compute product/2 safely to avoid overflow in small builds */
    if ((nonprime_n & 1) == 0) {
        return ((seq_t)(nonprime_n / 2)) * (seq_t)nonprime_np1;
    } else {
        return ((seq_t)(nonprime_np1 / 2)) * (seq_t)nonprime_n;
    }
}
