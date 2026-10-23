/*
 * A045948  a(n) = A003418(n)/A034386(n).
 * https://oeis.org/A045948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = A003418(n)/A034386(n) * = product_{p ≤ n} p^{⌊log_p n⌋ −
 *            1} * = product_{k≥2} A034386(⌊n^{1/k}⌋) (alternative form) * For
 *            n < 1 the function returns 0 (undefined, but never called)
 * Range:     u8 n <= 26, u16 n <= 120, u32 n <= 288, u64 n <= 1368,
 *            u128 n <= 5040, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"   /* provides build_primes_upto */

static uint64_t *prime_tbl  = NULL;   /* array of primes ≤ prime_limit */
static size_t    prime_cnt  = 0;       /* number of primes in prime_tbl   */
static idx_t     prime_limit = 0;     /* current maximum n for which
                                      * primes have been generated     */

/* Build (or rebuild) the list of primes up to 'limit' inclusive.
 * The list is kept in prime_tbl and prime_cnt is updated.
 * Previously allocated memory is freed before replacing it. */
static void add_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;   /* already have primes up to this limit */

    uint64_t *new_tbl = NULL;
    size_t    new_cnt = 0;
    build_primes_upto((uint64_t)limit, &new_tbl, &new_cnt);

    if (prime_tbl)
        free(prime_tbl);

    prime_tbl   = new_tbl;
    prime_cnt   = new_cnt;
    prime_limit = limit;
}

/* a(n) as defined above.  It works for any seq_t (built‑in
 * integer or arbitrary‑precision) and any idx_t (uint64_t). */
seq_t a(idx_t n)
{
    if (n < 1)           /* not defined for n < 1; avoid infinite loops */
        return (seq_t)0;

    add_primes_upto(n);   /* ensure primes up to n are available */

    seq_t result = (seq_t)1;
    seq_t n_seq  = (seq_t)n;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p64    = prime_tbl[i];
        if (p64 > (uint64_t)n)   /* stop when primes exceed n */
            break;

        seq_t p_seq  = (seq_t)p64;
        seq_t power  = p_seq;    /* initially p^1 */

        /* Find the largest power of p that does not exceed n. */
        while (true)
        {
            seq_t next = power * p_seq;   /* p^{e+1} */
            if (next <= n_seq)
                power = next;
            else
                break;
        }

        /* If the exponent in lcm(1..n) is e (e ≥ 1), we need p^{e-1}.
         * power == p^e.  Subtract one exponent to get p^{e-1}. */
        if (power > p_seq)          /* e > 1  ⇒  factor > 1 */
        {
            seq_t factor = power / p_seq;   /* p^{e-1} */
            result *= factor;
        }
    }

    return result;
}
