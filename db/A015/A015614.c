/*
 * A015614  a(n) = -1 + Sum_{i=1..n} phi(i).
 * https://oeis.org/A015614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -1 + Sum_{i=1..n} phi(i)
 * Range:     u8 n <= 28, u16 n <= 463, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_N 200000

static seq_t sum_arr[MAX_N + 1];
static idx_t phi_arr[MAX_N + 1];
static idx_t primes[MAX_N + 1];
static size_t primes_cnt = 0;
static idx_t max_n = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    if (n > max_n)
    {
        if (n > MAX_N)
        {
            /* compute on the fly if the pre‑allocated limit is exceeded */
            seq_t acc = 0;
            for (idx_t i = 1; i <= n; ++i)
            {
                idx_t x = i;
                idx_t phi = i;
                for (idx_t p = 2; (uint64_t)p * p <= x; ++p)
                {
                    if (x % p == 0)
                    {
                        phi = phi / p * (p - 1);
                        while (x % p == 0)
                            x /= p;
                    }
                }
                if (x > 1)
                    phi = phi / x * (x - 1);
                acc += (seq_t)phi;
            }
            return acc - 1;
        }

        for (idx_t i = max_n + 1; i <= n; ++i)
        {
            idx_t value = i;
            idx_t x = i;
            for (size_t k = 0; k < primes_cnt; ++k)
            {
                idx_t p = primes[k];
                if (p * p > x)
                    break;
                if (x % p == 0)
                {
                    value = value / p * (p - 1);
                    while (x % p == 0)
                        x /= p;
                }
            }
            if (x > 1)
                value = value / x * (x - 1);

            phi_arr[i] = value;

            /* remember primes for future factorisations */
            if (i >= 2 && value == i - 1)
                primes[primes_cnt++] = i;

            sum_arr[i] = sum_arr[i - 1] + (seq_t)value;
        }
        max_n = n;
    }

    return sum_arr[n] - (seq_t)1;
}
