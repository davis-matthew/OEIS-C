/*
 * A052180  Last filtering prime for n-th prime p: find smallest prime factor
 *          of each of the composite numbers between p and next prime; take
 *          maximal value.
 * https://oeis.org/A052180
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = max_{p_n < j < p_{n+1}} least_prime_factor(j), where p_k
 *            is the k‑th prime. Off‑set is 2 (a(2) is the first listed term).
 *            Implementation uses incremental prime sieving and trial division
 *            for the least prime factor. It works for any integer type seq_t
 *            (including big‑int) and idx_t (uint64_t)
 * Range:     u8 n <= 6594, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */
/*  Required headers – minimal set for also compiling under C++.     */
/* ------------------------------------------------------------------ */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Static storage for the list of primes (0‑based: pr[0] = 2).       */
/* ------------------------------------------------------------------ */
static uint64_t *pr = NULL;     /* dynamic array of primes */
static size_t   prime_cnt = 0;  /* number of primes stored */

/* ------------------------------------------------------------------ */
/*  Ensure that the prime array contains at least (idx+1) primes. Tag  */
/*  for idx=0 -> stake 1 prime (2).  This function is called only     */
/*  with idx >= 0.                                                     */
/* ------------------------------------------------------------------ */
static void ensure_primes_to_index(idx_t idx)
{
    if (prime_cnt > idx)
        return;                    /* already enough primes */

    /* First candidate to test */
    uint64_t candidate;
    if (prime_cnt == 0) {
        candidate = 2;
    } else {
        candidate = pr[prime_cnt - 1] + 1;
        if ((candidate & 1) == 0)  /* keep it odd after 2 */
            candidate++;
    }

    /* Grow the array in chunks; realloc each time one more is needed */
    while (prime_cnt <= idx) {
        /* Test primality using already known primes */
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt && pr[i] * pr[i] <= candidate; ++i) {
            if (candidate % pr[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            uint64_t *tmp = (uint64_t *)realloc((void *)pr,
                                 (prime_cnt + 1) * sizeof(uint64_t));
            if (!tmp)
                abort();  /* Out of memory – terminate */
            pr = tmp;
            pr[prime_cnt++] = candidate;
        }

        /* Advance to next odd number (after 2) */
        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;
    }
}

/* ------------------------------------------------------------------ */
/*  Least prime factor using the current prime list.  This function   */
/*  assumes candidates are <= UINT64_MAX.  Returns the smallest     */
/*  prime divisor; returns the number itself if it is prime.         */
/* ------------------------------------------------------------------ */
static uint64_t least_prime_factor(uint64_t n)
{
    if ((n & 1) == 0)          /* even number */
        return 2;

    /* Ensure primes cover sqrt(n) */
    uint64_t limit = (uint64_t)(n >> 1);   /* just a safe over‑estimate */
    while (pr[prime_cnt - 1] * pr[prime_cnt - 1] <= limit) {
        /* Need more primes */
        uint64_t cand = pr[prime_cnt - 1] + 2;
        bool prime = true;
        for (size_t i = 0; i < prime_cnt && pr[i] * pr[i] <= cand; ++i) {
            if (cand % pr[i] == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            uint64_t *tmp = (uint64_t *)realloc((void *)pr,
                                 (prime_cnt + 1) * sizeof(uint64_t));
            if (!tmp)
                abort();
            pr = tmp;
            pr[prime_cnt++] = cand;
        } else {
            cand += 2;
        }
    }

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = pr[i];
        if (p * p > n)
            break;
        if (n % p == 0)
            return p;
    }
    return n;   /* n is prime */
}

/* ------------------------------------------------------------------ */
/*  Main sequence function.  Computes a(n) for n >= 2.                */
/*  Outside the offset (n < 2) we return zero.                        */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;   /* value outside OEIS offset */

    /* Ensure we have primes up to index n (zero‑based). */
    ensure_primes_to_index(n);

    uint64_t pn        = pr[n - 1];   /* prime(n)   */
    uint64_t pn_next   = pr[n];      /* prime(n+1) */

    seq_t max_lpf = 0;               /* maximum least prime factor */

    for (uint64_t j = pn + 1; j < pn_next; ++j) {
        uint64_t lpf = least_prime_factor(j);
        if (lpf == j)                /* j itself is prime – skip */
            continue;
        if ((seq_t)lpf > max_lpf)
            max_lpf = (seq_t)lpf;    /* promote to seq_t type */
    }

    return max_lpf;
}
