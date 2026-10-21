/*
 * A072891  The 5-cycle of the n => sigma(n)-n process, where sigma(n) is the
 *          sum of divisors of n (A000203).
 * https://oeis.org/A072891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(1) = 12496; for n > 1, a(n) = sigma(a(n‑1)) - a(n‑1). The
 *            sequence forms a 5‑cycle, so a(5 + k) = a(k)
 * Range:     u8 none, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t sigma(seq_t n)
{
    seq_t sum = (seq_t)1;          /* 1 is always a divisor */
    seq_t i;
    for (i = (seq_t)2; i <= n / i; ++i)  /* i*i <= n, safe for the values involved */
    {
        if (n % i == 0)
        {
            sum += i;
            seq_t j = n / i;
            if (j != i) sum += j;
        }
    }
    if (n > 1) sum += n;          /* add the number itself */
    return sum;
}

seq_t a(idx_t n)
{
    static seq_t cycle[5];
    static int init = 0;

    if (!init)
    {
        seq_t cur = (seq_t)12496;          /* a(1) */
        int i;
        for (i = 0; i < 5; ++i)
        {
            cycle[i] = cur;
            cur = sigma(cur) - cur;       /* aliquot sum σ(n) – n */
        }
        init = 1;
    }
    return cycle[(n - (idx_t)1) % (idx_t)5];
}
