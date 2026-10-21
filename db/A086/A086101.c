/*
 * A086101  Numbers j such that the concatenation of the last digit of p(j)
 *          and the first digit of prime(j+1) is a prime.
 * https://oeis.org/A086101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 117, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: compute A086101
 * a(n) = the n-th integer j such that the number obtained by
 * concatenating the last digit of the j‑th prime and the first digit
 * of the (j+1)‑th prime is a prime.
 * Implementation: generate primes sequentially and test the
 * concatenated two‑digit number.  Results are cached so that
 * repeated calls in any order are efficient.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* helper functions ------------------------------------------------------ */

/* first decimal digit of a positive integer */
static inline int first_digit(uint64_t x)
{
    while (x >= 10) x /= 10;
    return (int)x;
}

/* primality test for numbers < 100 (used for the concatenated 2‑digit number) */
static inline bool is_prime_small(int x)
{
    if (x < 2) return false;
    for (int d = 2; d * d <= x; ++d)
        if (x % d == 0) return false;
    return true;
}

/* ----------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /*  static state for prime generation and caching of results  */
    static uint64_t *primes = NULL;      /* list of primes found so far   */
    static size_t prime_cap       = 0;   /* allocated capacity of primes  */
    static size_t prime_cnt       = 0;   /* number of primes in the list  */
    static uint64_t next_candidate = 2;  /* candidate for next prime       */

    if (n == 0) return 0; /* undefined for n==0 – offset is 1 */

    static idx_t terms_needed = 0; /* number of terms computed till now */
    static idx_t last_checked_j = 0; /* j index already evaluated (1‑based) */
    static seq_t *terms = NULL;  /* cached qualifying j values   */
    static size_t terms_cap = 0;
    static size_t terms_cnt = 0;

    /* ensure terms vector large enough for the requested n */
    if (n > terms_cnt) {
        while (terms_cnt < n) {
            /* generate next prime and store it   */
            if (prime_cnt == 0) {
                /* first prime */
                if (prime_cap == 0) {
                    prime_cap = 16;
                    primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
                }
                primes[prime_cnt++] = 2;
                next_candidate = 3;
            } else {
                /* find next prime by trial division */
                for (;;) {
                    bool ok = true;
                    for (size_t i = 0; i < prime_cnt; ++i) {
                        uint64_t p = primes[i];
                        if (p * p > next_candidate) break;
                        if (next_candidate % p == 0) { ok = false; break; }
                    }
                    if (ok) break;
                    next_candidate += 2;
                }
                if (prime_cnt == prime_cap) {
                    prime_cap *= 2;
                    primes = (uint64_t *)realloc(primes,
                            prime_cap * sizeof(uint64_t));
                }
                primes[prime_cnt++] = next_candidate;
                next_candidate += 2;
            }

            /* after storing a new prime, we can test j = prime_cnt-1 (1‑based) */
            if (prime_cnt >= 2) {
                uint64_t pj      = primes[prime_cnt - 2]; /* p(j)   */
                uint64_t pj_next = primes[prime_cnt - 1]; /* p(j+1) */
                int concat = 10 * (int)(pj % 10) + first_digit(pj_next);

                if (is_prime_small(concat)) {
                    /* store j */
                    if (terms_cap == terms_cnt) {
                        terms_cap = terms_cap ? terms_cap * 2 : 16;
                        terms = (seq_t *)realloc(terms,
                               terms_cap * sizeof(seq_t));
                    }
                    terms[terms_cnt++] = (seq_t)(prime_cnt - 1); /* j 1‑based */
                }
            }
        }
    }

    return terms[n - 1];
}
