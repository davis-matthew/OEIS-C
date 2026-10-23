/*
 * A187758  Number of ways to write n=x+y (x,y>0) with 2x-3, 2x+3, 6y+1 and
 *          6y+5 all prime.
 * https://oeis.org/A187758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8826, u16 n <= 8826, u32 n <= 8826, u64 n <= 8826,
 *            u128 n <= 8826, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A187758: Number of ways to write n = x + y (x,y>0)
   with 2x-3, 2x+3, 6y+1 and 6y+5 all prime. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Allocate a simple prime sieve up to a limit. */
static bool *prime_table = NULL;
static idx_t prime_limit = 0;

static void ensure_primes_upto(idx_t lim)
{
    if (lim <= prime_limit)
        return;

    /* Allocate new table */
    size_t sz = (size_t)lim + 1;
    bool *new_table = (bool *)calloc(sz, sizeof(bool));
    if (!new_table)
        return; /* allocation failure – give up */

    for (idx_t i = 0; i <= lim; i++)
        new_table[i] = true;

    new_table[0] = false;
    if (lim >= 1)
        new_table[1] = false;

    for (idx_t i = 2; i * i <= lim; i++) {
        if (new_table[i]) {
            for (idx_t j = i * i; j <= lim; j += i)
                new_table[j] = false;
        }
    }

    free(prime_table);
    prime_table = new_table;
    prime_limit = lim;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Determine upper bound needed for prime sieve */
    idx_t primemax = 6 * (n - 1) + 5;
    if (2 * n + 3 > primemax)
        primemax = 2 * n + 3;

    ensure_primes_upto(primemax);

    seq_t count = 0;
    for (idx_t x = 1; x < n; x++) {
        idx_t y  = n - x;
        idx_t p1 = 2 * x - 3;
        if (p1 < 2)
            continue;          /* 2x-3 must be at least 2 to be prime */

        idx_t p2 = 2 * x + 3;
        idx_t p3 = 6 * y + 1;
        idx_t p4 = 6 * y + 5;

        if (prime_table[p1] && prime_table[p2] &&
            prime_table[p3] && prime_table[p4]) {
            count++;
        }
    }
    return count;
}
