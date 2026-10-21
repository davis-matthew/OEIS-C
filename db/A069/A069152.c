/*
 * A069152  a(n) = (n-1)!-n^tau(n)/n^2.
 * https://oeis.org/A069152
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n-1)! - n^(tau(n)-2) (offset 2)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* tau(n) = number of divisors of n; n ≥ 2 */

#include <stdlib.h>
#include <stdint.h>

/* Compute τ(n), the number of positive divisors of n (n ≥ 2). */
static uint64_t divisor_count_u64(uint64_t n)
{
    uint64_t cnt = 0;
    for (uint64_t i = 1; i * i <= n; ++i)
        if (n % i == 0) {
            uint64_t j = n / i;
            cnt += (i == j) ? 1 : 2;
        }
    return cnt;
}

/* Integer power for seq_t using binary exponentiation. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* a(n) = (n-1)! - n^(τ(n)-2),  n ≥ 2. */
seq_t a(idx_t n)
{
    /* Compute (n‑1)! directly. */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n - 1; ++i)
        fact *= (seq_t)i;

    /* Compute τ(n) and the power n^(τ(n)-2). */
    uint64_t tau = divisor_count_u64((uint64_t)n);
    idx_t exp = tau - 2;            /* τ(n) ≥ 2 for n ≥ 2 → exp ≥ 0 */
    seq_t pow = pow_seq_t((seq_t)n, exp);

    /* Result (non‑negative for all n ≥ 2). */
    return fact - pow;
}
