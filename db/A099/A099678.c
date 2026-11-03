/*
 * A099678  Numbers n such that 10n01 is a prime.
 * https://oeis.org/A099678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9874 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that the decimal concatenation 10 n 01 is prime.
   Implementation: incremental search storing found terms for subsequent
   queries.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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
    /* static storage for generated sequence values */
    static seq_t *terms = NULL;      /* dynamically expanded array of n */
    static size_t terms_len = 0;     /* number of terms stored */
    static size_t terms_cap = 0;     /* allocated capacity */
    static idx_t next_candidate = 1; /* next n to test */

    /* extend storage until we have at least n terms */
    while (terms_len < (size_t)n)
    {
        /* determine number of decimal digits of current candidate */
        int d = (int)num_digits_idx(next_candidate);

        /* build the number 10n01 */
        seq_t candidateN =
            (seq_t)10 * pow10((idx_t)(d + 2)) + (seq_t)next_candidate * 100 + (seq_t)1;

        /* test primality of the constructed number */
        if (is_prime_big(candidateN))
        {
            /* grow array if necessary */
            if (terms_len == terms_cap)
            {
                terms_cap = terms_cap ? terms_cap * 2 : 16;
                terms = mem_resize(seq_t, terms, terms_cap);
            }
            terms[terms_len++] = next_candidate;
        }

        /* advance to next candidate n */
        next_candidate++;
    }

    /* return the nth term (1‑based index) */
    return terms[(size_t)n - 1];
}
