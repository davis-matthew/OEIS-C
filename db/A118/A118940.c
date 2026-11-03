/*
 * A118940  Primes p such that (p^2+7)/8 is prime.
 * https://oeis.org/A118940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A118940: primes p such that (p^2+7)/8 is prime.
   The function returns the nth term (indexing starts at 1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Below the offset behaviour is irrelevant; return 0 if requested. */
    if (n == 0) return 0;

    /* Static cache of terms found so far. */
    static seq_t *terms      = NULL;   /* array of sequence values */
    static size_t terms_capacity = 0;   /* allocated capacity */
    static size_t terms_count    = 0;   /* number of terms stored */

    /* Candidate generation state. */
    static uint64_t next_candidate = 3; /* start from 3 and step by 2 (odd) */

    /* Generate terms until we have at least n. */
    while (terms_count < n) {
        /* Find the next prime candidate > 2. */
        uint64_t p;
        do {
            p = next_candidate;
            next_candidate += 2;          /* only odd numbers */
        } while (!is_prime_seq((seq_t)p)); /* fast small‑prime test */

        /* Compute (p^2 + 7) / 8 using 128‑bit intermediate to avoid overflow. */
        unsigned __int128 prod   = (unsigned __int128)p * p;
        unsigned __int128 q128   = (prod + 7) / 8;
        seq_t            q       = (seq_t)q128;      /* cast back to seq_t */

        /* Check that the derived number is prime. */
        if (is_prime_big(q)) {
            /* Ensure enough space in the cache. */
            if (terms_count == terms_capacity) {
                terms_capacity = terms_capacity ? terms_capacity * 2 : 16;
                terms = mem_resize(seq_t, terms, terms_capacity);
            }
            /* Store the new term. */
            terms[terms_count++] = (seq_t)p;
        }
    }

    return terms[n - 1];
}
