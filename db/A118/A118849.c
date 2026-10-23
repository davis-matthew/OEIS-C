/*
 * A118849  Digital clock primes such that the number of minutes past midnight
 *          is also prime.
 * https://oeis.org/A118849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   a(n) = A118850(n) + floor(A118850(n)/60)*40
 * Range:     u8 n <= 24, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/*  A118850(n) is the n‑th minute past midnight that is prime and whose
    decimal representation HHMM is also prime. */

static seq_t seq_cache[1440];
static idx_t seq_len;
static bool seq_inited = false;

/* Simple primality test for numbers <= 2359 (fits in idx_t). */
static bool is_prime_small(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Build the sequence once. */
static void init_sequence(void)
{
    if (seq_inited) return;
    seq_inited = true;
    seq_len = 0;

    for (idx_t hour = 0; hour < 24; ++hour)
    {
        for (idx_t minute = 0; minute < 60; ++minute)
        {
            idx_t total_minutes = hour * 60 + minute;
            if (!is_prime_small(total_minutes)) continue;
            idx_t hhmm = hour * 100 + minute;
            if (!is_prime_small(hhmm)) continue;
            seq_cache[seq_len++] = (seq_t)hhmm;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for offset 0 */
    init_sequence();
    if (n > seq_len) return (seq_t)0; /* beyond the finite list */
    return seq_cache[n - 1];
}
