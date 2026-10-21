/*
 * A062976  Half-totient function of n is divisible by the number of divisors
 *          of n.
 * https://oeis.org/A062976
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Condition: (phi(m)/2) divisible by d(m).  a(n) = the n‑th integer
   m ≥ 3 satisfying the condition (offset 3).  For n < 3 the function
   returns 0. */
#include <stdint.h>
#include <stdbool.h>

#define MAXN 2000000ULL   /* enough to contain the first 10 000 terms */

static uint32_t phi[MAXN + 1];
static uint32_t d_cnt[MAXN + 1];
static uint32_t spf[MAXN + 1];
static uint8_t  exp_spf[MAXN + 1];
static uint32_t primes[200000];
static uint32_t primes_cnt = 0;
static bool initialized = false;

/* Linear sieve that fills phi[], d_cnt[], spf[], exp_spf[] and primes[] */
static void init_sieve(void)
{
    phi[1] = 1;
    d_cnt[1] = 1;
    spf[1] = 1;
    exp_spf[1] = 0;

    for (uint32_t i = 2; i <= MAXN; ++i)
    {
        if (spf[i] == 0)                    /* i is prime */
        {
            spf[i]       = i;
            exp_spf[i]   = 1;
            phi[i]       = i - 1;
            d_cnt[i]     = 2;
            primes[primes_cnt++] = i;
        }

        for (uint32_t j = 0; j < primes_cnt; ++j)
        {
            uint32_t p   = primes[j];
            uint64_t ip  = (uint64_t)i * p;
            if (ip > MAXN)
                break;
            spf[ip] = p;
            if (p == spf[i])                 /* p divides i */
            {
                exp_spf[ip] = exp_spf[i] + 1;
                phi[ip]     = phi[i] * p;
                d_cnt[ip]   = d_cnt[i] / (exp_spf[i] + 1) *
                               (exp_spf[ip] + 1);
            }
            else                             /* p does not divide i */
            {
                exp_spf[ip] = 1;
                phi[ip]     = phi[i] * (p - 1);
                d_cnt[ip]   = d_cnt[i] * 2;
            }
            if (spf[i] < p)                   /* maintain linearity */
                break;
        }
    }
    initialized = true;
}

/* Returns the n‑th term of OEIS A062976 (offset 3).  If n < 3, 0 is returned. */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    if (!initialized)
        init_sieve();

    idx_t target = n - 2;   /* a(3) is the first such integer */
    idx_t found  = 0;
    for (idx_t i = 3; i <= MAXN; ++i)
    {
        if (((phi[i] / 2) % d_cnt[i]) == 0)
        {
            ++found;
            if (found == target)
                return (seq_t)i;
        }
    }
    return 0;   /* Should never be reached for the supported range of n */
}
