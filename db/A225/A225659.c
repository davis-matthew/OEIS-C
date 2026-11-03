/*
 * A225659  Primes p where p + sumOfDigits(p) +- 3 is prime.
 * https://oeis.org/A225659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, dumb
 * Formula:   A225659 – primes p with p + sumDigits(p) ± 3 prime
 * Range:     u8 n <= 25, u16 n <= 394, u32 n <= 394, u64 n <= 394,
 *            u128 n <= 394, big any n
 * Validated: standard, pass, 62 terms through n = 376 (2026-11-03)
 * Helpers:   digit_sum, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a memoized list of terms.  New terms
   are generated on demand by testing successive primes for the
   required property.  It works for 8,16,32,64 or 128‑bit unsigned
   seq_t and for arbitrary‑precision seq_t as supplied by the build.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
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

/* Returns the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* static state: memoised terms, current candidate, capacity */
    static seq_t *terms = NULL;
    static idx_t term_count = 0;      /* how many terms have been found */
    static idx_t cap = 0;            /* current capacity of terms array   */
    static seq_t next_candidate = 2; /* next integer to examine as prime */

    /* allocate / grow terms array lazily */
    if (terms == NULL) {
        cap = 16;
        terms = mem_new(seq_t, cap);
        if (!terms) abort();
    }

    /* generate terms until we have at least n of them */
    while (term_count < (idx_t)n) {
        seq_t cand = next_candidate;

        /* test cand for primality */
        if (is_prime_big(cand)) {
            seq_t sod = digit_sum(cand);
            seq_t q1 = cand + sod + 3;
            seq_t q2 = cand + sod - 3;

            /* if p±3 property holds, store cand */
            if (is_prime_big(q1) || is_prime_big(q2)) {
                if ((idx_t)term_count + 1 > cap) {
                    /* double capacity */
                    size_t new_cap = cap ? cap * 2 : 16;
                    seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                    if (!tmp) abort();
                    terms = tmp;
                    cap = new_cap;
                }
                terms[term_count++] = cand;
            }
        }

        /* advance to next candidate */
        if (cand == 2)
            next_candidate = 3;   /* after 2 start at 3 */
        else
            next_candidate += 2; /* skip even numbers */
    }

    return terms[(idx_t)n - 1];
}
