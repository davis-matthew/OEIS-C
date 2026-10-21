/*
 * A073612  Group the positive integers as (1, 2), (3, 4, 5), (6, 7, 8, 9,
 *          10), (11, 12, 13, 14, 15, 16, 17), ... the n-th group containing
 *          prime(n) elements. Except the first, all groups contain an odd
 *          number of elements and hence have a middle term. Sequence gives
 *          the middle terms starting from group 2.
 * https://oeis.org/A073612
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^{n-1} prime(i) + 1 + (prime(n)-1)/2 (n ≥ 2)
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static storage for primes and their cumulative sums */
static idx_t *primes = NULL;      /* primes[0] = 2, primes[1] = 3, … */
static seq_t *cum   = NULL;      /* cum[i] = sum of primes[0..i]   */
static idx_t prime_cnt   = 0;     /* number of primes stored         */
static idx_t next_cand   = 2;     /* next integer to test for prime */

/* Ensure that at least 'needed' primes are stored */
static void ensure_primes(idx_t needed)
{
    if (prime_cnt >= needed) return;

    /* enlarge buffers if necessary */
    idx_t cap = prime_cnt ? prime_cnt << 1 : 4;
    while (cap < needed) cap <<= 1;

    if (!primes) {
        primes = (idx_t *)malloc(cap * sizeof(idx_t));
        cum    = (seq_t *)malloc(cap * sizeof(seq_t));
    } else {
        primes = (idx_t *)realloc(primes, cap * sizeof(idx_t));
        cum    = (seq_t *)realloc(cum,    cap * sizeof(seq_t));
    }

    while (prime_cnt < needed) {
        idx_t cand = next_cand++;

        /* skip even numbers > 2 */
        if (cand > 2 && (cand & 1ULL) == 0) continue;

        /* trial division using known primes */
        bool is_prime = true;
        for (idx_t i = 0; i < prime_cnt && primes[i] <= cand / primes[i]; ++i) {
            if (cand % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            primes[prime_cnt] = cand;
            cum[prime_cnt] = (prime_cnt ? cum[prime_cnt - 1] : 0) + (seq_t)cand;
            ++prime_cnt;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* Undefined for n < 2 (not used by harness) */

    ensure_primes(n);               /* make sure primes[n-1] exists */

    /* sum of first n-1 primes */
    seq_t sum_before = cum[n - 2];   /* valid for all n >= 2 */

    /* middle offset within group n */
    seq_t mid_off = ((seq_t)primes[n - 1] - 1) / 2;

    return 1 + sum_before + mid_off;
}
