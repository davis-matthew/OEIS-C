/*
 * A121823  (3^p+p)/(p+1) with (p + 1) odd prime > 3.
 * https://oeis.org/A121823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A121823: a(n) = (3^p + p) / (p + 1) where (p + 1) is the
   n‑th odd prime greater than 3.  Using the (n+2)‑nd prime.  */

#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;      /* primes up to the current limit */
static size_t prime_cnt    = 0;          /* how many primes are stored */

/* return a rough upper bound that contains at least target_cnt primes */
static idx_t estimate_prime_limit(idx_t target_cnt)
{
    if (target_cnt < 6)
        return 15;                   /* prime_5 = 11, more than enough */
    double n = (double)target_cnt;
    double limit = n * (log(n) + log(log(n)));
    return (idx_t)(limit + 10.0);     /* safety margin */
}

/* ensure prime_list contains at least 'needed_count' primes (1‑based) */
static void
ensure_primes(idx_t needed_count)
{
    if (prime_list && prime_cnt >= (size_t)needed_count)
        return;                     /* already sufficient */

    idx_t limit = estimate_prime_limit(needed_count);
    uint64_t *new_list = NULL;
    size_t new_cnt = 0;
    build_primes_upto(limit, &new_list, &new_cnt);
    if (prime_list)
        free(prime_list);
    prime_list = new_list;
    prime_cnt  = new_cnt;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;             /* offset is 1 */

    /* we need the (n+2)-nd prime (1‑based).  zero‑based index = n+1. */
    idx_t target_prime_index = n + 2;          /* 1‑based */
    ensure_primes(target_prime_index);

    uint64_t prime = prime_list[target_prime_index - 1];  /* prime > 3, odd */
    uint64_t p = prime - 1;                           /* p = prime-1 */

    /* ----------------- 64‑bit path ---------------------- */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 pow128 = 1;
        for (idx_t i = 0; i < p; ++i)
            pow128 *= 3;
        unsigned __int128 numerator = pow128 + (unsigned __int128)p;
        unsigned __int128 result = numerator / prime;
        return (seq_t)result;      /* fits in 64‑bit for all tests that fit */
    }

    /* ----------------- arbitrary‑precision path ---------- */
    seq_t pow_val = big_pow((seq_t)3, (seq_t)p);
    seq_t numer   = pow_val + (seq_t)p;
    return numer / (seq_t)prime;
}
