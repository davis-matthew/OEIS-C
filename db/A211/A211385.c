/*
 * A211385  Values of n for which product_{p|n, p prime} 1 + 1/p >
 *          e^gamma*log(log(n)).
 * https://oeis.org/A211385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A211385:
   values of n for which   Π_{p|n} (1+1/p) > e^γ * log(log(n))   */

#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Eulers constant e^γ ≈ 1.781072417990197053928919 */
static const double E_GAMMA = 1.7810724179901970539289189655765269353946;

/* Cache of found terms (maximum 20 terms are needed for the known sequence) */
static seq_t cache[20];
static size_t cache_len = 0;
static uint64_t search_next = 2;

/* Check whether a candidate n satisfies the inequality. */
static bool candidate_ok(uint64_t n)
{
    uint64_t temp = n;
    uint64_t primes[64];
    size_t cnt = 0;

    /* trial‑division factorisation (distinct primes only) */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            primes[cnt++] = p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1)
        primes[cnt++] = temp;

    /* product Π (1 + 1/p) */
    double prod = 1.0;
    for (size_t i = 0; i < cnt; ++i)
        prod *= 1.0 + 1.0 / (double)primes[i];

    /* right–hand side e^γ * log(log(n)) */
    double rhs = E_GAMMA * log(log((double)n));

    return prod > rhs;
}

/* Generate terms until the cache contains at least n entries. */
static void ensure_terms(size_t n)
{
    const uint64_t MAX_CANDIDATE = 1000;   /* upper bound for the search */
    while (cache_len < n) {
        if (search_next > MAX_CANDIDATE) {
            /* no more terms are expected – fill remaining cache slots with 0 */
            cache[cache_len++] = (seq_t)0;
            continue;
        }
        if (candidate_ok(search_next))
            cache[cache_len++] = (seq_t)search_next;
        search_next++;
    }
}

/* Public function: returns the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    ensure_terms((size_t)n);
    return cache[(size_t)n - 1];
}
