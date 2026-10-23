/*
 * A163857  Number of sexy prime quadruples (p, p+6, p+12, p+18), with p <= n.
 * https://oeis.org/A163857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of sexy prime quadruples (p, p+6, p+12, p+18) with
 *            p ≤ n
 * Range:     u8 n <= 9683, u16 n <= 9683, u32 n <= 9683, u64 n <= 9683,
 *            u128 n <= 9683, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cum_counts = NULL;   /* cumulative counts, cum_counts[i] = a(i) */
static idx_t   cum_cap    = 0;      /* maximum index for which cum_counts is valid */

/* Recompute sieve and cumulative counts up to at least 'limit' (inclusive). */
static void compute_upto(idx_t limit)
{
    if (limit <= cum_cap) return;

    /* Allocate sieve of size limit+1 */
    size_t sz = (size_t)limit + 1U;
    uint8_t *is_prime = (uint8_t *)malloc(sz);
    if (!is_prime) exit(1);                 /* allocation failure – abort */

    memset(is_prime, 1, sz);                /* assume all prime initially */
    if (sz > 0) is_prime[0] = 0;
    if (sz > 1) is_prime[1] = 0;

    /* Classic Eratosthenes sieve */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p) {
                is_prime[j] = 0;
            }
        }
    }

    /* Allocate new cumulative array */
    seq_t *new_cum = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!new_cum) { free(is_prime); exit(1); }
    new_cum[0] = (seq_t)0;

    idx_t cumulative = 0U;
    for (idx_t i = 1; i <= limit; ++i) {
        if (i <= limit - 18U &&
            is_prime[i]  && is_prime[i + 6U] &&
            is_prime[i + 12U] && is_prime[i + 18U]) {
            ++cumulative;
        }
        new_cum[i] = (seq_t)cumulative;
    }

    /* Replace old data */
    free(cum_counts);
    free(is_prime);
    cum_counts = new_cum;
    cum_cap    = limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* out of range, undefined per spec */
    idx_t needed = n + 18U;       /* ensure we can count all quadruples ≤ n */
    compute_upto(needed);
    return cum_counts[n];
}
