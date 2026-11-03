/*
 * A089058  Each positive number x is repeated m times, where there are m
 *          numbers y >= x such that x*y/(x+y) is an integer.
 * https://oeis.org/A089058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 830, u16 n <= 10010, u32 n <= 10010, u64 n <= 10010,
 *            u128 n <= 10010, big any n
 * Validated: standard, pass, 74 terms through n = 9979 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A089058.
   Each positive integer x ≥ 2 appears m times, where m is
   the number of divisors of x² that are ≤ x/2.
   This implementation:
   1. Uses a static SPF sieve (up to 2 000 000) for fast factorization.
   2. Recursively counts the admissible divisors for each x.
   3. Accumulates repetitions until the desired index n is reached. */

#include <stdint.h>
#include <stdlib.h>

/* Recursive divisor counter.
   current  – product of chosen prime powers so far
   idx      – current prime index
   pc       – total number of distinct primes in x
   primes   – array of prime bases
   exps     – array of exponents in x
   limit    – highest divisor allowed (x/2) as seq_t                  */
static unsigned count_div_rec(seq_t current, unsigned idx, unsigned pc,
                              const unsigned *primes, const unsigned *exps,
                              seq_t limit)
{
    if (idx == pc)                       /* finished building a divisor   */
        return (current <= limit) ? 1u : 0u;

    unsigned total = 0u;
    seq_t ppow = 1;                      /* prime^k for current k        */
    unsigned maxk = 2 * exps[idx];
    for (unsigned k = 0; k <= maxk; ++k) {
        seq_t newcurrent = current * ppow;
        if (newcurrent > limit) break;   /* further k will only grow   */
        total += count_div_rec(newcurrent, idx + 1, pc,
                               primes, exps, limit);

        /* prepare next power, guarding against overflow (seq_t may be 64‑bit) */
        if (k == maxk) break;
        if (ppow > limit / (seq_t)primes[idx]) break;
        ppow *= (seq_t)primes[idx];
    }
    return total;
}

static unsigned *spf = NULL;          /* smallest prime factor array */
static idx_t spf_limit = 0;           /* current size of spf array */

static void init_spf(idx_t limit)
{
    if (spf) return;                 /* already initialised */
    spf_limit = limit;
    spf = (unsigned *)malloc((size_t)(spf_limit + 1) * sizeof(unsigned));
    for (size_t i = 0; i <= (size_t)spf_limit; ++i) spf[i] = (unsigned)i;
    for (size_t i = 2; i * i <= (size_t)spf_limit; ++i)
        if (spf[i] == (unsigned)i)
            for (size_t j = i * i; j <= (size_t)spf_limit; j += i)
                if (spf[j] == (unsigned)j) spf[j] = (unsigned)i;
}

seq_t a(idx_t n)
{
    init_spf(2000000);                   /* enough for n up to 10 000 */

    idx_t cumulative = 0;
    for (idx_t x = 2; ; ++x) {
        /* factorise x with SPF */
        idx_t tmp = x;
        unsigned primes[32];
        unsigned exps[32];
        unsigned pc = 0;

        while (tmp > 1) {
            unsigned p = spf[tmp];
            unsigned e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            primes[pc] = p;
            exps[pc]   = e;
            ++pc;
        }

        /* count admissible divisors of x² */
        seq_t limit = (seq_t)(x / 2);
        unsigned m = count_div_rec((seq_t)1, 0, pc, primes, exps, limit);

        if (cumulative + m >= n)
            return (seq_t)x;

        cumulative += m;
    }
}
