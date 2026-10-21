/*
 * A089724  Numbers p satisfying the following conditions: p is a prime of
 *          form 10k+9 and the function f[p,7,10] applied to p is not greater
 *          than 5, where
 *          f[p,7,10]=10*fractionalpart[n/70]=1*.((n/70)-Floor[n/70]).
 * https://oeis.org/A089724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A089724:  primes p with p == 9 (mod 10) and (p % 70) <= 35
   (equivalently, f[p,7,10] = 10 * (p/70 - ⌊p/70⌋) ≤ 5). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *primes   = NULL;   /* cache of qualifying primes          */
static size_t primes_len   = 0;   /* number stored in `primes`          */
static size_t primes_cap   = 0;   /* allocated capacity of `primes`     */
static uint64_t last_scan  =  9; /* last candidate examined (pre‑19)   */

/* Ensure capacity for at least `need` elements. */
static void ensure_capacity(size_t need)
{
    if (primes_cap >= need)
        return;
    size_t newcap = primes_cap ? primes_cap * 2 : 8;
    while (newcap < need)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(primes, newcap * sizeof (seq_t));
    if (!tmp)
        abort();                     /* memory allocation failure         */
    primes = tmp;
    primes_cap = newcap;
}

/* Extend the cache until it holds at least `n` terms. */
static void extend_to(idx_t n)
{
    if (primes_len >= (size_t)n)
        return;

    size_t need = (size_t)n;
    ensure_capacity(need);

    while (primes_len < need) {
        uint64_t cand = last_scan + 10ULL;  /* next number ending in 9   */
        last_scan = cand;
        if ((cand % 70ULL) <= 35ULL && is_prime(cand)) {
            primes[primes_len++] = (seq_t)cand;
        }
    }
}

/* Return the n-th term (1‑based).  Undefined for n < 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    extend_to(n);
    return primes[n - 1];
}
