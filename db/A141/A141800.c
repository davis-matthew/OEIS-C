/*
 * A141800  For n >= 2, a(n) = Sum_{k=1..n} (smallest prime dividing (n*k)).
 *          a(1)=0.
 * https://oeis.org/A141800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (smallest prime dividing (n*k))
 * Range:     u8 n <= 36, u16 n <= 886, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *spf = NULL;     /* smallest prime factor table */
static idx_t   spf_max = 0;

/* Builds or extends the spf array up to `limit`.  After the call
   spf[i] contains the smallest prime factor of i for all 2 ≤ i ≤ limit. */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_max)
        return;

    size_t new_size = (size_t)limit + 1;
    uint64_t *new_spf = (uint64_t *)realloc(spf, new_size * sizeof(uint64_t));
    if (!new_spf) abort();
    spf = new_spf;

    /* initialise new entries to themselves */
    for (idx_t i = spf_max + 1; i <= limit; ++i)
        spf[i] = i;

    /* compute smallest prime factors */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == (uint64_t)i) {           /* i is prime */
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == j) spf[j] = i;
            }
        }
    }
    spf_max = limit;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    ensure_spf(n);

    uint64_t p = spf[n];          /* smallest prime divisor of n */
    seq_t sum = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        uint64_t spfk = (k == 1) ? p : spf[k];
        seq_t val = (spfk < p) ? (seq_t)spfk : (seq_t)p;
        sum += val;
    }
    return sum;
}
