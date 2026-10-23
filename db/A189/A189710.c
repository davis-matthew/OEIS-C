/*
 * A189710  Numbers m such that m'' = m'-1 where m' and m'' are respectively
 *          the first and the second arithmetic derivative of m (A003415).
 * https://oeis.org/A189710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 6552, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A189710:
   Numbers m such that m'' = m' - 1,
   where m' and m'' are the first and second arithmetic derivatives. */
/* The algorithm generates the sequence by testing integers in ascending
   order.  All computations are performed with 64‑bit integers; the
   result is then cast to seq_t.  This works for the 64‑bit build and for
   the arbitrary‑precision build (where the cast invokes the appropriate
   constructor). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- prime generation ---------- */
static uint64_t *primes = NULL;          /* array of primes */
static size_t     prime_cnt   = 0;        /* how many primes we have */
static size_t     prime_cap   = 0;        /* allocated capacity */

/* add the next prime to the primes array */
static void add_next_prime(void)
{
    if (prime_cap == 0) {
        prime_cap = 16;
        primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    } else if (prime_cnt == prime_cap) {
        prime_cap *= 2;
        primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
    }

    uint64_t candidate = 2;
    if (prime_cnt > 0) candidate = primes[prime_cnt - 1] + 1;

    for (;;) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            primes[prime_cnt++] = candidate;
            break;
        }
        ++candidate;
    }
}

/* ensure we have all primes up to at least limit (inclusive) */
static void ensure_primes_up_to(uint64_t limit)
{
    if (prime_cnt == 0) add_next_prime();   /* start with 2 */

    while (primes[prime_cnt - 1] < limit)
        add_next_prime();
}

/* ---------- arithmetic derivative for uint64_t ---------- */
static uint64_t arith_deriv_uint64(uint64_t m)
{
    if (m <= 1) return 0;

    uint64_t tmp = m;
    uint64_t sum = 0;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            sum += e * (m / p);
        }
    }
    if (tmp > 1)                       /* remaining prime factor, exponent 1 */
        sum += m / tmp;

    return sum;
}

/* ---------- sequence cache ---------- */
static seq_t *terms   = NULL;          /* stored terms of the sequence */
static size_t terms_cap = 0;           /* capacity of the terms array */
static size_t max_n   = 0;             /* number of terms currently stored */
static uint64_t next_m = 2;            /* next integer to test */

/* generate the sequence up to at least target_n terms */
static void generate_up_to(idx_t target_n)
{
    if ((idx_t)max_n >= target_n) return;

    /* allocate storage for terms if necessary */
    if (terms == NULL) {
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    while ((idx_t)max_n < target_n) {
        /* test candidate m = next_m */
        uint64_t m = next_m++;
        ensure_primes_up_to(m);

        uint64_t first  = arith_deriv_uint64(m);
        uint64_t second = arith_deriv_uint64(first);

        if (second == first - 1) {
            if (terms_cap <= max_n) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[max_n++] = (seq_t)m;   /* cast to seq_t */
        }
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined below offset */
    generate_up_to(n);
    return terms[n - 1];
}
