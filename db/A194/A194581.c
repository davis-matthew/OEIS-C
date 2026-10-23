/*
 * A194581  Primes prime(k) of the form (2*prime(k-1) + prime(k+1))/3.
 * https://oeis.org/A194581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 502, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A194581:
 *   a(n) = prime(k) such that (2*prime(k-1)+prime(k+1))/3 == prime(k)
 *   offset = 1 (a(1) = prime(2) = 3)
 *
 * The function generates primes incrementally and records the indices
 * of primes that satisfy the relation.  The nth term is then
 * retrieved from the primes array.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- Prime generation data ---------- */
static uint64_t *primes     = NULL;  /* list of generated primes   */
static size_t   primes_cnt = 0;      /* number of primes stored    */
static size_t   primes_cap = 0;      /* allocated capacity         */

/* ---------- Term indices data (index into primes array) ---------- */
static size_t *term_idx     = NULL;  /* indices of primes that are sequence terms */
static size_t  term_cnt     = 0;     /* number of terms stored            */
static size_t  term_cap     = 0;     /* allocated capacity                */

/* ---------- Helper: append a prime to list ---------- */
static void push_prime(uint64_t val)
{
    /* Resize prime array if necessary */
    if (primes_cnt == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) abort();          /* out of memory */
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_cnt++] = val;

    /* Test the middle prime once we have at least three */
    if (primes_cnt >= 3) {
        uint64_t prevprev = primes[primes_cnt - 3];
        uint64_t prev     = primes[primes_cnt - 2];
        uint64_t next     = val;

        /* 2*prevprev + next == 3*prev  ⇔  prev is a sequence term */
        if (2 * prevprev + next == 3 * prev) {
            /* Resize term index array if necessary */
            if (term_cnt == term_cap) {
                size_t new_cap = term_cap ? term_cap * 2 : 16;
                size_t *tmp = (size_t *)realloc(term_idx, new_cap * sizeof(size_t));
                if (!tmp) abort();
                term_idx = tmp;
                term_cap = new_cap;
            }
            /* Store index of the qualifying prime (prev) */
            term_idx[term_cnt++] = primes_cnt - 2;
        }
    }
}

/* ---------- Helper: primality test (trial division) ---------- */
static int is_prime_candidate(uint64_t cand)
{
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > cand / p) break;      /* p*p > cand */
        if (cand % p == 0) return 0;   /* composite */
    }
    return 1;                          /* prime */
}

/* ---------- Ensure we have at least "need" terms ---- */
static void ensure_terms(idx_t need)
{
    while (term_cnt < need) {
        if (primes_cnt == 0) {           /* first prime */
            push_prime(2);
            continue;
        }
        if (primes_cnt == 1) {           /* second prime */
            push_prime(3);
            continue;
        }

        /* Find next prime */
        uint64_t cand = primes[primes_cnt - 1] + 2;   /* skip even numbers */
        while (!is_prime_candidate(cand))
            cand += 2;
        push_prime(cand);
    }
}

/* ---------- Public function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1; not expected to be called */
    ensure_terms(n);
    uint64_t prime_val = primes[term_idx[n - 1]];
    return (seq_t)prime_val;
}
