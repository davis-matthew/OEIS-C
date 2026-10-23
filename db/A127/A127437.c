/*
 * A127437  Duplicate of A001915.
 * https://oeis.org/A127437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 18, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th prime p for which there exists m with 2^m ≡ 3 (mod m),
   equivalently 3 is a power of 2 modulo p.  The definition uses only 
   the condition: 3^(ord_p(2)) ≡ 1 (mod p). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

static seq_t *seq_cache = NULL;          /* cache of found terms */
static size_t seq_len  = 0;              /* number of terms cached */
static size_t seq_cap  = 0;              /* capacity of seq_cache    */
static idx_t  next_prime_candidate = 3;  /* next prime to test       */

/* resize the cache array to at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    /* realloc may return void*; cast to seq_t* for C++ compatibility */
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* store a new term in the cache */
static void add_term(seq_t val)
{
    ensure_capacity(seq_len + 1);
    seq_cache[seq_len++] = val;
}

/* compute the n-th term of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* out-of-range, behaviour undefined */

    /* initialise cache with first term */
    if (seq_len == 0) {
        add_term((seq_t)2);              /* a(1) = 2 */
        next_prime_candidate = 3;
    }

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        /* find next prime candidate */
        while (!is_prime(next_prime_candidate)) ++next_prime_candidate;
        idx_t p = next_prime_candidate; /* current prime */

        /* ---------- compute ord_p(2) ---------- */
        idx_t ord = p - 1;               /* p is odd for all n>1 */
        idx_t temp = ord;
        idx_t factors[64];
        size_t nf = 0;

        /* factorise p-1 using trial division up to sqrt(temp) */
        for (idx_t f = 2; f * f <= temp; ++f) {
            if (temp % f == 0) {
                factors[nf++] = f;
                while (temp % f == 0) temp /= f;
            }
        }
        if (temp > 1 && nf < 64) factors[nf++] = temp;

        /* reduce ord by each prime factor */
        for (size_t i = 0; i < nf; ++i) {
            idx_t f = factors[i];
            while (ord % f == 0 &&
                   mod_pow((seq_t)2, (uint64_t)(ord / f), (seq_t)p) == (seq_t)1) {
                ord /= f;
            }
        }

        /* ---------- test if 3 is a power of 2 modulo p ---------- */
        if (mod_pow((seq_t)3, (uint64_t)ord, (seq_t)p) == (seq_t)1) {
            add_term((seq_t)p);  /* p satisfies the condition */
        }

        ++next_prime_candidate;    /* proceed to next candidate */
    }

    return seq_cache[n - 1];
}
