/*
 * A227983  Numbers n such that sigma(n+1) - sigma(n-1) < sigma(n); where
 *          sigma(n) = A000203(n) = sum of the divisors of n.
 * https://oeis.org/A227983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 239, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Condition: sigma(n+1) - sigma(n-1) < sigma(n)  (A227983) */
#include <stdbool.h>
#include <stdint.h>

static seq_t sigma_of_uint64(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 0;
    idx_t i;
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            sum += (seq_t)i;
            idx_t j = n / i;
            if (j != i)
                sum += (seq_t)j;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* defensive, sequences are 1‑indexed */
    idx_t count = 0;
    idx_t i = 2;               /* start testing from n = 2 */
    for (;;) {
        seq_t sigma_prev = sigma_of_uint64(i - 1);
        seq_t sigma_cur  = sigma_of_uint64(i);
        seq_t sigma_next = sigma_of_uint64(i + 1);
        if (sigma_next < sigma_prev ||
            (sigma_next - sigma_prev) < sigma_cur) {
            ++count;
            if (count == n)
                return (seq_t)i;
        }
        ++i;
    }
}
