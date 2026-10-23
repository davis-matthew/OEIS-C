/*
 * A103272  a(n) = (prime(n-k) + prime(n)) mod 4, where k = (1/2)*(prime(n+1)
 *          - prime(n)).
 * https://oeis.org/A103272
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A103272
 * Formula: a(n) = (prime(n-k) + prime(n)) mod 4,
 * where k = (prime(n+1) - prime(n)) / 2
 * (prime(1) = 2, prime(2) = 3, …)
 * The sequence is defined for n ≥ 2; a(n) for n < 2 is undefined.
 */

#include <stdint.h>
#include <stdlib.h>

/*  Dynamic cache of primes (1‑based: prime[1] = 2).  */
static uint64_t *primes = NULL;   /* array of primes */
static idx_t     primes_cnt = 0;  /* number of primes stored   */
static idx_t     primes_cap = 0;  /* current array capacity    */

/*  Ensure that primes[1..need] are generated.   */
static void
ensure_primes_up_to(idx_t need)
{
    if (primes_cnt >= need)
        return;

    /* Grow the array if necessary.  */
    if (primes_cap < need) {
        idx_t new_cap = primes_cap ? primes_cap * 2 : 8;
        if (new_cap < need)
            new_cap = need;
        /* Allocate capacity for indices 0..new_cap   */
        uint64_t *tmp = (uint64_t *)realloc(primes, (new_cap + 1) * sizeof(uint64_t));
        if (tmp == NULL) {
            /* Allocation failure – abort.  */
            abort();
        }
        primes = tmp;
        primes_cap = new_cap;
    }

    /*  Generate primes from the first missing one  */
    uint64_t candidate;
    if (primes_cnt == 0)
        candidate = 2;
    else
        candidate = primes[primes_cnt] + 1;

    /* Make sure candidate starts as an odd number (except 2). */
    if (candidate > 2 && candidate % 2 == 0)
        candidate++;

    while (primes_cnt < need) {
        /* Test primality of candidate.  */
        int is_prime = 1;
        for (idx_t i = 1; i <= primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p > candidate / p) /* p*p would overflow or exceed candidate */
                break;
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            primes[++primes_cnt] = candidate;
            if (primes_cnt == need)
                break;
        }

        /* Advance to next candidate: skip even numbers. */
        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;
    }
}

/*  Sequence function  */
seq_t
a(idx_t n)
{
    if (n < 2)            /* invalid index – return 0 for safety */
        return (seq_t)0;

    /*  Need primes up to index n+1.  */
    ensure_primes_up_to(n + 1);

    uint64_t p_n   = primes[n];
    uint64_t p_n1  = primes[n + 1];
    uint64_t diff  = p_n1 - p_n;          /* always even */
    idx_t    k     = diff / 2;
    idx_t    idx   = n - k;               /* should be ≥ 1 for n ≥ 2 */

    uint64_t p_nk  = primes[idx];
    uint64_t mod4  = (p_nk + p_n) & 3ULL; /* modulo 4 */

    return (seq_t)mod4;
}
