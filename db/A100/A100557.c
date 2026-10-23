/*
 * A100557  Smallest prime of the form (prime(n)*prime(n+1)+q)/2 for some
 *          integer n and some prime q.
 * https://oeis.org/A100557
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   isprime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A100557:  a(n) = smallest prime of the form (prime(n)*prime(n+1)+q)/2
   with q a prime (odd).  For n ≥ 2.  The function searches for the
   first odd prime q such that the resulting value is prime.  The list of
   primes is built incrementally by trial division. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isprime_big.h"

/*--- Prime table, grown dynamically ------------------------------------*/
static seq_t *prime_tbl = NULL;          /* array of primes (0‑based)   */
static idx_t   prime_cnt = 0;            /* current number of primes    */
static idx_t   prime_cap = 0;            /* current allocated capacity  */

/* Append a prime, expanding buffer as necessary */
static void add_prime(seq_t p)
{
    if (prime_cnt == prime_cap) {
        idx_t new_cap = prime_cap ? prime_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(prime_tbl, new_cap * sizeof(seq_t));
        prime_tbl = tmp;
        prime_cap = new_cap;
    }
    prime_tbl[prime_cnt++] = p;
}

/* Generate primes until we have at least 'need' primes.
   Uses trial division by previously found primes. */
static void primes_until(idx_t need)
{
    if (prime_cnt >= need)
        return;

    /* Seed the list if empty */
    if (prime_cnt == 0) {
        add_prime(2);               /* prime(1) */
        if (need <= 1)
            return;
        add_prime(3);               /* prime(2) */
    }

    seq_t candidate = prime_tbl[prime_cnt - 1] + 2; /* next odd */
    while (prime_cnt < need) {
        bool is_p = true;
        uint64_t cand = (uint64_t)candidate;
        for (idx_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = (uint64_t)prime_tbl[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p)
            add_prime(candidate);
        candidate += 2;
    }
}

/*--- Sequence function -----------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 2)                /* offset is 2, undefined below */
        return 0;

    /* Ensure primes p_n and p_{n+1} are available (indexing starts at 1) */
    primes_until(n + 1);      /* need prime(n) and prime(n+1) */

    const seq_t p  = prime_tbl[n - 1];    /* prime(n)   */
    const seq_t p1 = prime_tbl[n];        /* prime(n+1) */

    /* 128‑bit product to avoid overflow before division */
    unsigned __int128 prod = (unsigned __int128)p * (unsigned __int128)p1;

    /* Search over odd primes q until (prod+q)/2 is prime */
    idx_t q_index = 1;          /* start with prime 3 (index 1) */
    while (true) {
        primes_until(q_index + 1);        /* ensure prime at index q_index */
        seq_t q = prime_tbl[q_index];    /* odd prime (>=3) */
        unsigned __int128 sum = prod + (unsigned __int128)q;
        if ((sum & 1) == 0) {           /* sum must be even to divide by 2 */
            seq_t val = (seq_t)(sum >> 1);   /* (sum/2) */
            if (isprime_big(val))
                return val;
        }
        ++q_index;
    }
}
