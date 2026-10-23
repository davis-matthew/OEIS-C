/*
 * A171016  In the sequence of prime numbers, replace all the '3' digits with
 *          '0' and vice versa.
 * https://oeis.org/A171016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth prime with all decimal digits 3 replaced by 0 and all
 *            0 replaced by 3 (swap 3<->0)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t *prime_tbl  = NULL;   /* dynamic array of primes */
static size_t prime_cnt  = 0;      /* number of primes stored */
static size_t prime_cap  = 0;      /* allocated capacity of prime_tbl */

/* Expand prime_tbl to at least 'size' elements. */
static void
ensure_prime_capacity(size_t size)
{
    if (size <= prime_cap)
        return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    if (new_cap < size)
        new_cap = size;
    idx_t *new_tbl = (idx_t *)realloc(prime_tbl, new_cap * sizeof(idx_t));
    if (!new_tbl) abort();
    prime_tbl = new_tbl;
    prime_cap = new_cap;
}

/* Generate the next prime and store it in prime_tbl.  Assumes prime_tbl already
   has the correct capacity and prime_cnt is the current number of primes. */
static void
add_next_prime(void)
{
    if (prime_cnt == 0) {
        ensure_prime_capacity(1);
        prime_tbl[0] = 2;
        prime_cnt = 1;
        return;
    }

    idx_t candidate = prime_tbl[prime_cnt - 1] + 1;
    /* Skip even candidates beyond 2 */
    if (candidate > 2 && (candidate & 1) == 0)
        ++candidate;

    for (;;) {
        bool is_pr = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_tbl[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_pr = false;
                break;
            }
        }
        if (is_pr) {
            ensure_prime_capacity(prime_cnt + 1);
            prime_tbl[prime_cnt++] = candidate;
            return;
        }
        candidate += 2;  /* skip even numbers */
    }
}

/* Ensure that prime_tbl contains at least 'need' primes. */
static void
ensure_primes(size_t need)
{
    while (prime_cnt < need)
        add_next_prime();
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0)   /* offset is 1 */
        return 0;

    /* We need at least n primes (indexed from 1). */
    ensure_primes((size_t)n);

    idx_t p = prime_tbl[n - 1];

    seq_t result  = 0;
    seq_t factor  = 1;
    while (p > 0) {
        unsigned d = (unsigned)(p % 10);
        if (d == 3)
            d = 0;
        else if (d == 0)
            d = 3;
        result += (seq_t)d * factor;
        factor *= 10;
        p /= 10;
    }
    return result;
}
