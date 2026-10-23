/*
 * A167339  Totally multiplicative sequence with a(p) = p*(p-2) = p^2-2p for
 *          prime p.
 * https://oeis.org/A167339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e || n of (p*(p-2))^e, with
 *            a(1)=1 and a(2k)=0 for k≥1
 * Range:     u8 n <= 18, u16 n <= 262, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Smallest prime factor table */
static uint32_t *spf = NULL;            /* nullptr until initialized   */
static idx_t     spf_limit = 0;         /* current maximum spf index   */

/* Initialise/extend spf table up to `limit`.  Complexity: O(limit log log limit). */
static void init_spf(idx_t limit)
{
    if (limit <= spf_limit) return;            /* already enough */

    /* allocate new table */
    uint32_t *new_spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!new_spf) abort();                     /* insufficient memory */

    /* initialise with zeros */
    for (idx_t i = 0; i <= limit; ++i) new_spf[i] = 0;

    /* Smallest prime factor sieve */
    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {                 /* i is prime */
            new_spf[i] = (uint32_t)i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    if (new_spf[j] == 0) new_spf[j] = (uint32_t)i;
                }
            }
        }
    }

    /* free old table and store the new one */
    free(spf);
    spf        = new_spf;
    spf_limit  = limit;
}

seq_t a(idx_t n)
{
    /* a(1) is the first term */
    if (n == 1) return (seq_t)1;

    /* all even numbers (>=2) give 0 */
    if ((n & 1ULL) == 0) return (seq_t)0;

    /* initialise smallest‑prime‑factor table if needed */
    init_spf(n);

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* factorise n using the SPF table */
    while (m > 1) {
        uint32_t p = spf[m];
        int e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        seq_t factor = (seq_t)p * (seq_t)(p - 2);
        for (int i = 0; i < e; ++i) {
            result *= factor;
        }
    }

    return result;
}
