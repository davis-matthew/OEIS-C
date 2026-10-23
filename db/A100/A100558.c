/*
 * A100558  Primes of the form [prime(n)*prime(n+1)+p]/2 with increasing p.
 * https://oeis.org/A100558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime_uint64, isprime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A100558 – primes of the form (prime(n+1)*prime(n+2)+p)/2,
 *  where p is the smallest prime > the previous p that makes the
 *  average prime.  The sequence is indexed from 1.  Calculation is
 *  via a simple search with precomputed primes.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"   /* for is_prime_uint64() */
#include "../utils/isprime_big.h"      /* for isprime_big() */

/* dynamic array of primes, 0‑based: primes[0] = 2 */
static uint64_t *prime_list = NULL;
static idx_t prime_count = 0;
static idx_t prime_cap   = 0;

/* ensure primes[0 .. limit] exist (inclusive) */
static void ensure_primes(idx_t limit)
{
    while (prime_count <= limit) {
        uint64_t cand;
        if (prime_count == 0)
            cand = 2;
        else
            cand = prime_list[prime_count - 1] + 1;

        /* skip even numbers except 2 */
        if (cand > 2 && cand % 2 == 0)
            cand++;

        /* find the next prime */
        for (;;) {
            if (is_prime_uint64(cand)) {
                if (prime_count >= prime_cap) {
                    prime_cap = prime_cap ? prime_cap * 2 : 32;
                    void *tmp = realloc(prime_list, prime_cap * sizeof(uint64_t));
                    if (!tmp) abort();
                    prime_list = (uint64_t *)tmp;
                }
                prime_list[prime_count++] = cand;
                break;
            }
            cand += 2;      /* test next odd number */
        }
    }
}

/* dynamic array of sequence values */
static seq_t *seq = NULL;
static idx_t seq_count = 0;
static idx_t seq_cap   = 0;

/* append a term to seq array */
static void seq_append(seq_t v)
{
    if (seq_count >= seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 16;
        void *tmp = realloc(seq, seq_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq = (seq_t *)tmp;
    }
    seq[seq_count++] = v;
}

/* main function */
seq_t a(idx_t n)
{
    static uint64_t last_p = 0;   /* p of previous term (0 for n=1) */

    if (n == 0) return (seq_t)0; /* undefined for offset 1 */

    /* generate terms up to index n */
    while (seq_count < n) {
        idx_t k = seq_count + 1;             /* 1‑based index for this term */
        ensure_primes(k + 1);                /* need primes[k] and primes[k+1] */

        seq_t prod = (seq_t)prime_list[k] * (seq_t)prime_list[k + 1];

        /* start searching for p > last_p, p must be odd >=3 */
        uint64_t p = last_p + 1;
        if (p <= 2)          /* skip even prime 2 */
            p = 3;
        else if (p % 2 == 0)
            p++;              /* make it odd */

        for (;;) {
            /* p is guaranteed to be odd; find next prime if needed */
            while (!is_prime_uint64(p))
                p += 2;

            seq_t val = (prod + (seq_t)p) / 2;
            if (isprime_big(val)) {
                /* a valid term found */
                last_p = p;
                seq_append(val);
                break;
            }
            p += 2;            /* next odd candidate for p */
        }
    }

    return seq[n - 1];
}
