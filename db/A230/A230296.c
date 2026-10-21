/*
 * A230296  a(n) = Sum_{i=1..n} d(72*i+1) - Sum_{i=1..n} d(18*i+1), where d(n)
 *          = A000005(n).
 * https://oeis.org/A230296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} d(72*i+1) - Sum_{i=1..n} d(18*i+1)
 * Range:     u8 n <= 541, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d(k) = number of divisors of k */

#include <stdlib.h>
#include <stdint.h>

static uint32_t *spf = NULL;          /* smallest prime factor array */
static idx_t    spf_limit = 0;        /* largest index for which spf is valid */

/* Build prime sieve to supply smallest prime factors up to 'limit'. */
static void build_spf(idx_t limit)
{
    if (limit <= spf_limit) return;

    /* free any previous allocation */
    free(spf);

    /* allocate array of size limit+1 */
    spf = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
    if (!spf) return; /* allocation failed – caller will see NULL */

    /* classic sieve to fill spf */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i > limit / i) continue; /* avoid overflow of i*i */

            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }

    spf_limit = limit;
}

/* Number of divisors of n using previously built spf. */
static seq_t divisor_count(idx_t n)
{
    if (n == 0) return 0;          /* never used but keeps API safe */
    if (n == 1) return 1;

    seq_t total = 1;
    idx_t temp = n;

    while (temp > 1) {
        idx_t p = spf[temp];
        if (p == 0) p = temp;     /* should not happen if sieve ran up to n */
        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        total *= (seq_t)(exp + 1);
    }
    return total;
}

/* Public function computing the desired sequence value. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for 0 but safe guard */

    /* Determine the largest number we might need to evaluate d(k) for. */
    idx_t max_needed = 72 * n + 1;
    build_spf(max_needed);

    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        sum += divisor_count(72 * i + 1);
        sum -= divisor_count(18 * i + 1);
    }
    return sum;
}
