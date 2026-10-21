/*
 * A062610  Number of ways of writing n = c1 + c2 with c1 and c2 nonprimes [=1
 *          or composite].
 * https://oeis.org/A062610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..⌊n/2⌋} A005171(i)*A005171(n-i)
 * Range:     u8 n <= 689, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A005171(i) is 1 if i is non‑prime (i = 1 or composite), 0 otherwise. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Prime sieve flags: 1 = prime, 0 = non‑prime (including 0 and 1) */
static uint8_t *prime_flag = NULL;    /* dynamic array of flags */
static idx_t    prime_flag_size = 0;  /* last valid index in prime_flag */

/* Ensure that prime_flag[0..n] is valid, filling missing entries with a sieve */
static void ensure_prime_flag(idx_t n)
{
    if (prime_flag_size >= n)          /* already large enough */
        return;

    /* Reallocate to hold flags up to n (inclusive) */
    void *tmp = realloc(prime_flag, sizeof(uint8_t) * (n + 1));
    if (!tmp) /* allocation failed – keep the old cache */
        return;
    prime_flag      = (uint8_t *)tmp;
    prime_flag_size = n;

    /* Initialise all entries as prime, then clear 0 and 1 */
    for (idx_t i = 0; i <= n; ++i)
        prime_flag[i] = 1;
    prime_flag[0] = 0;
    if (n >= 1)
        prime_flag[1] = 0;

    /* Classic Sieve of Eratosthenes up to n */
    for (idx_t p = 2; p * p <= n; ++p)
    {
        if (prime_flag[p])
        {
            for (idx_t j = p * p; j <= n; j += p)
                prime_flag[j] = 0;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1)            /* behaviour for indices below the offset is irrelevant */
        return 0;

    ensure_prime_flag(n);

    seq_t count = 0;
    for (idx_t i = 1; i <= n / 2; ++i)
    {
        idx_t j = n - i;
        if (!prime_flag[i] && !prime_flag[j])
            ++count;
    }
    return count;
}
