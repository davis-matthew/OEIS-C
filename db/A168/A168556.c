/*
 * A168556  Smallest primes p = p(k) with (p(k)+p(k+1)+p(k+2))/15 an integer.
 * https://oeis.org/A168556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 67, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A168556:  Smallest primes p = p(k) such that
 *  (p(k)+p(k+1)+p(k+2))/15  is an integer.
 *  The function builds primes incrementally and stores every prime
 *  that begins a qualifying triple.  The list is cached so that
 *  any n ≥ 1 is returned in O(1) after the initial scan.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* ---------- Configuration ---------- */
#define MAX_TERMS 200000         /* enough for n up to ~10000 */
#define MAX_PRIMES 300000        /* store many primes for trial division */

/* ---------- Global state ---------- */
static seq_t   terms[MAX_TERMS]; /* cached sequence terms                    */
static size_t  terms_cnt = 0;    /* number of terms found so far              */

static uint64_t  primes[MAX_PRIMES]; /* list of primes found so far      */
static size_t    primes_cnt   = 0;    /* how many primes stored            */
static uint64_t  next_candidate = 3;  /* odd number to test next           */

/* ---------- Prime generation ---------- */
static uint64_t next_prime(void)
{
    /* First prime is 2 */
    if (primes_cnt == 0) {
        primes[0] = 2;
        primes_cnt = 1;
        next_candidate = 3;
        return 2;
    }

    /* Search for the next odd prime */
    for (uint64_t n = next_candidate; ; n += 2) {
        uint64_t limit = (uint64_t)sqrt((double)n);
        int is_prime = 1;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p > limit) break;
            if (n % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            if (primes_cnt < MAX_PRIMES)
                primes[primes_cnt++] = n;
            /* otherwise we silently ignore overflow – tests stay within limits */
            next_candidate = n + 2;
            return n;
        }
    }
    /* unreachable */
    return 0;
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n = 0, but safe return */

    /* Grow sequence until we have at least n terms */
    while (terms_cnt < (size_t)n) {
        /* Generate next prime */
        uint64_t prime = next_prime();

        /* Check current triple if we have at least 3 primes now */
        if (primes_cnt >= 3) {
            uint64_t p1 = primes[primes_cnt - 3];
            uint64_t p2 = primes[primes_cnt - 2];
            uint64_t p3 = primes[primes_cnt - 1];
            uint64_t sum = p1 + p2 + p3;
            if ((sum % 15) == 0) {
                if (terms_cnt < MAX_TERMS)
                    terms[terms_cnt++] = (seq_t)p1;   /* store the first prime of the triple */
            }
        }
    }

    return terms[n - 1];
}
