/*
 * A056268  Number of primitive (aperiodic) words of length n which contain
 *          exactly three different symbols.
 * https://oeis.org/A056268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * (3^{n/d} - 3*2^{n/d} + 3)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* Smallest prime factor table, grows as needed */
static int *spf = NULL;
static size_t spf_size = 0;

static void init_spf(size_t limit)
{
    size_t new_size = limit + 1;
    spf = (int *)realloc(spf, new_size * sizeof(int));
    for (size_t i = 0; i <= limit; ++i)
        spf[i] = (int)i;
    size_t sq = (size_t)(limit * limit);
    for (size_t i = 2; i * i <= limit; ++i) {
        if (spf[(size_t)i] == (int)i) {            /* i is prime */
            for (size_t j = i * i; j <= limit; j += i)
                if (spf[(size_t)j] == (int)j)
                    spf[(size_t)j] = (int)i;
        }
    }
    spf_size = new_size;
}

static int mobius_value(idx_t x)
{
    if (x == 1) return 1;
    int d = 0;
    while (x > 1) {
        int p = spf[(size_t)x];
        int e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
            if (e > 1) return 0;          /* square factor -> μ=0 */
        }
        ++d;
    }
    return (d & 1) ? -1 : 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Ensure spf table covers n */
    if (n + 1 > spf_size)
        init_spf((size_t)n);

    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t k = n / d;
            seq_t term = pow_seq(3, k) - 3 * pow_seq(2, k) + 3;
            int mu_d = mobius_value(d);
            result += (seq_t)mu_d * term;

            if (d != k) {
                idx_t e = k;
                seq_t term2 = pow_seq(3, n / e) - 3 * pow_seq(2, n / e) + 3; /* equals term with k ... */
                int mu_e = mobius_value(e);
                result += (seq_t)mu_e * term2;
            }
        }
    }
    return result;
}
