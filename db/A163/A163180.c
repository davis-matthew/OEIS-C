/*
 * A163180  a(n) = tau(n) + Sum_{k=1..n} (n mod k).
 * https://oeis.org/A163180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n) + sum_{k=1..n} (n mod k)
 * Range:     u8 n <= 38, u16 n <= 608, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* tau(n) counts the positive divisors of n. */

#include <stdint.h>

/* Return the number of positive divisors of n. */
static seq_t divisor_count(idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            if (i * i == n)
                count += 1;
            else
                count += 2;
        }
    }
    return count;
}

/* Return sum_{k=1..n} (n mod k).  Computed in O(sqrt(n)) time. */
static seq_t sum_remainders(idx_t n)
{
    seq_t sum_floor = 0;
    idx_t l = 1;
    while (l <= n)
    {
        idx_t q = n / l;
        idx_t r = n / q;          /* largest k with floor(n/k) == q */
        idx_t cnt = r - l + 1;
        seq_t sum_k = (seq_t)(l + r) * cnt / 2; /* sum of k from l to r */
        sum_floor += (seq_t)q * sum_k;
        l = r + 1;
    }
    return (seq_t)n * n - sum_floor;
}

/* Sequence A163180: a(n) = tau(n) + sum_{k=1..n} (n mod k) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safe guard, though a(0) is unused */
    return divisor_count(n) + sum_remainders(n);
}
