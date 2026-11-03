/*
 * A072890  The 28-cycle of the n => sigma(n)-n process, where sigma(n) is the
 *          sum of divisors of n (A000203).
 * https://oeis.org/A072890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: full, pass, 29 terms through n = 29 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the 28-cycle of the sigma(n)-n process (aliquot chain) */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Local implementation of sigma(n): sum of all positive divisors of n */
static seq_t sigma_of_plain(idx_t n)
{
    if (n <= 1) return 1;     /* sigma(0)=0, sigma(1)=1 */
    seq_t sum = 1;
    idx_t remaining = n;
    for (idx_t p = 2; p * p <= remaining; ++p)
    {
        if (remaining % p == 0)
        {
            seq_t term = 1;
            seq_t pk = 1;
            while (remaining % p == 0)
            {
                remaining /= p;
                pk *= p;
                term += pk;
            }
            sum *= term;
        }
    }
    if (remaining > 1)        /* last prime factor > sqrt(n) */
        sum *= (1 + remaining);
    return sum;
}

/* Static storage for the 28-cycle; computed on first call */
static seq_t cycle[28];
static bool cycle_initialized = false;

static void init_cycle(void)
{
    if (cycle_initialized) return;
    seq_t cur = (seq_t)14316;              /* first element of the cycle */
    for (size_t i = 0; i < 28u; ++i)
    {
        cycle[i] = cur;
        seq_t sigma = sigma_of_plain((idx_t)cur);   /* sigma(cur) */
        cur = sigma - cur;                          /* next element */
    }
    cycle_initialized = true;
}

seq_t a(idx_t n)
{
    init_cycle();
    /* n is 1‑based; use modular arithmetic to reflect the 28‑term cycle */
    size_t pos = (size_t)((n - 1) % 28u);
    return cycle[pos];
}
