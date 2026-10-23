/*
 * A103959  Minimum prime p such that prime(n)*p - 2 is prime.
 * https://oeis.org/A103959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = minimal prime p such that prime(n)*p-2 is prime */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"

/* ------------------------------------------------------------------ */
/*  Prime generation – static array big enough for the first 12 000   */
/*  primes (required for n ≤ 10 000).                                  */
/* ------------------------------------------------------------------ */
#define MAX_PRIMES 12000

static seq_t prime_list[MAX_PRIMES];
static idx_t prime_cnt  = 0;      /* how many primes are stored          */
static idx_t last_checked = 1;    /* last integer tested for primality   */

/* Store a new prime in the table.  (Caller ensures we don't exceed */
/* the static buffer.)                                               */
static void add_prime(seq_t p)
{
    prime_list[prime_cnt++] = p;
}

/* Generate primes until we have at least n of them (i.e. until     */
/* prime(n) is available).                                           */
static void ensure_primes(idx_t n)
{
    while (prime_cnt < n)
    {
        ++last_checked;
        if (is_prime_seq((seq_t)last_checked))
            add_prime((seq_t)last_checked);
    }
}

/* --------------------------------------------------------------- */
/*  Main function:  a(n) – as defined by the OEIS entry            */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n=0 – return 0 as a placeholder */

    /* Make sure prime(n) is available. */
    ensure_primes(n);

    seq_t pn = prime_list[n - 1];   /* prime(n) */

    /* Find the smallest prime p ≤ prime(n) such that prime(n)*p‑2 is prime. */
    for (idx_t i = 0; i < prime_cnt; ++i)
    {
        seq_t p = prime_list[i];
        if (p > pn)
            break;              /* no larger p is allowed */
        seq_t val = pn * p - (seq_t)2;
        if (is_prime_seq(val))
            return p;
    }

    /* By the conjecture a(n) ≤ prime(n); return prime(n) if something goes wrong. */
    return pn;
}
