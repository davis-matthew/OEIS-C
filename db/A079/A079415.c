/*
 * A079415  a(n) = floor(prime(n)/n) * ceiling(prime(n)/n) / 2.
 * https://oeis.org/A079415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(prime(n)/n) * ceil(prime(n)/n) / 2
 * Range:     u8 n <= 54, u16 n <= 7421, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* 
 * Helper: generate primes on demand.  The list grows as needed.
 * Uses incremental trial division, which is fast enough for
 * the required limits (up to about 10 000‑th prime).
 */
static seq_t *prime_list = NULL;      /* array of primes */
static idx_t   prime_cnt  = 0;        /* number of primes stored */
static idx_t   prime_cap  = 0;        /* current capacity */

/* Ensure that prime_cnt >= needed */
static void
ensure_primes(idx_t needed)
{
    if (prime_cnt >= needed)
        return;

    if (prime_cap == 0) {
        prime_cap = 64;
        prime_list = (seq_t*)malloc(prime_cap * sizeof(seq_t));
    }

    /* Start searching from last prime + 1 or from 2 if list empty */
    unsigned long long cand = (prime_cnt == 0) ? 2ULL : (unsigned long long)(prime_list[prime_cnt - 1]) + 1ULL;

    /* Detect even numbers after 2 */
    if (cand > 2 && (cand & 1ULL) == 0ULL)
        cand++;

    while (prime_cnt < needed) {
        /* Trial division by existing primes up to sqrt(cand) */
        int is_prime = 1;
        for (idx_t i = 0; i < prime_cnt; ++i) {
            seq_t p = prime_list[i];
            if ((unsigned long long)p * (unsigned long long)p > cand)
                break;
            if (cand % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            /* Expand array if needed */
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                prime_list = (seq_t*)realloc(prime_list,
                                             prime_cap * sizeof(seq_t));
            }
            prime_list[prime_cnt] = (seq_t)cand;
            prime_cnt++;
        }

        cand += 2;  /* skip even numbers */
    }
}

/* Computes the OEIS sequence A079415 */
seq_t a(idx_t n)
{
    /* Offset is 1: a(1) is first term.  n==0 is undefined, return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Make sure we have at least n primes */
    ensure_primes(n);

    /* prime(n) stored at index n-1 */
    seq_t p = prime_list[n - 1];
    seq_t sn = (seq_t)n;      /* convert n to seq_t for division */

    seq_t q = p / sn;                 /* floor(p / n)          */
    seq_t r = (p + sn - 1) / sn;      /* ceil (p / n)          */
    return (q * r) / 2;               /* integer division truncates */
}
