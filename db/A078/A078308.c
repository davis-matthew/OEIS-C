/*
 * A078308  a(n) = Sum_{d divides n} d^(n/d + 1).
 * https://oeis.org/A078308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n/d + 1) (offset 1)
 * Range:     u8 n <= 9, u16 n <= 26, u32 n <= 59, u64 n <= 119, u128 n <= 239,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Exponentiation by squaring for arbitrary precision seq_t */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp != 0)
    {
        if (exp & 1)
            result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

/* Simple trial‑division prime factoriser for uint64_t */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p <= n / p; ++p)
    {
        if (n % p == 0)
        {
            uint64_t e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            pr[c] = p;
            exp[c] = e;
            ++c;
        }
    }
    if (n > 1)
    {
        pr[c] = n;
        exp[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Recursively enumerate all divisors of n and accumulate the sum */
static void enum_divs(const uint64_t *pr, const uint64_t *exp,
                      size_t cnt, uint64_t cur,
                      seq_t *sum, uint64_t n)
{
    if (cnt == 0)
    {
        seq_t d = (seq_t)cur;
        idx_t e = n / cur + 1;
        *sum += pow_seq_t(d, e);
        return;
    }
    uint64_t p = pr[0];
    uint64_t e = exp[0];
    uint64_t val = cur;
    for (uint64_t k = 0; k <= e; ++k)
    {
        enum_divs(pr + 1, exp + 1, cnt - 1, val, sum, n);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset 1 guarantees n>=1, guard anyway */
    uint64_t pr[64];
    uint64_t exp[64];
    size_t cnt;
    factor_uint64((uint64_t)n, pr, exp, &cnt);

    seq_t total = 0;
    enum_divs(pr, exp, cnt, 1, &total, (uint64_t)n);
    return total;
}
