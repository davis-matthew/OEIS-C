/*
 * A158635  6n - A008578(n).
 * https://oeis.org/A158635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, less
 * Formula:   a(n) = 6*n – p_{n-1} , where p_0 = 1 and for k ≥ 1, p_k is the
 *            k‑th prime (p_1 = 2)
 * Range:     i8 n <= 281, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* --------- prime cache ------------------------------------------- */

/* primes[0] = 2, primes[1] = 3, …  */
static uint64_t *primes      = NULL;
static size_t   primes_cnt   = 0;
static size_t   primes_cap   = 0;

/* grow the prime array when needed */
static void grow_prime_cache(size_t new_cap)
{
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();                      /* allocation failure */
    primes      = tmp;
    primes_cap  = new_cap;
}

/* add the next prime to the table */
static void add_next_prime(void)
{
    /* start at 2 or after the last stored prime */
    uint64_t cand = (primes_cnt == 0) ? 2ULL : primes[primes_cnt-1] + 1ULL;
    if (cand > 2 && (cand & 1ULL) == 0) cand++;   /* skip even numbers */

    while (1)
    {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i)
        {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) break;
        cand += 2; /* only test odd numbers */
    }

    if (primes_cnt >= primes_cap)
        grow_prime_cache(primes_cap ? primes_cap << 1 : 16);

    primes[primes_cnt++] = cand;
}

/* ensure that at least idx primes are present (idx >= 1) */
static void ensure_prime_count(size_t idx)
{
    while (primes_cnt < idx)
        add_next_prime();
}

/* return the prime at 0‑based index idx (0→2, 1→3, …) */
static uint64_t prime_at(size_t idx)
{
    if (idx == 0) return 2ULL;          /* first prime, no allocation needed */
    ensure_prime_count(idx + 1);
    return primes[idx];
}

/* --------- sequence implementation ---------------------------- */

seq_t a(idx_t n)
{
    /* convert n to seq_t first (as required) */
    seq_t sn = (seq_t)n;
    seq_t p  = 1;                      /* default p_0 */

    if (n > 1)                         /* for n ≥ 2, use (n‑1)‑th prime (1‑based) */
    {
        size_t prime_idx = (size_t)(n - 2);   /* 0‑based index in our array */
        p = (seq_t)prime_at(prime_idx);
    }

    return (seq_t)6 * sn - p;
}
