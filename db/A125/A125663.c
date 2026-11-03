/*
 * A125663  Numbers such that the left half of the digits form a prime and the
 *          right half do not.
 * https://oeis.org/A125663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the n‑th positive integer whose left half of digits
 * (floor(#digits/2) leading digits) is a prime and whose right half
 * (floor(#digits/2) trailing digits, ignoring the middle digit when the
 * number of digits is odd) is NOT a prime.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_digits_idx.h"
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Compute 10^exp using the index type (unsigned 64‑bit). */
static idx_t pow10_idx(int exp)
{
    idx_t r = 1;
    for (int i = 0; i < exp; ++i)
        r *= 10;
    return r;
}

/* Return the n-th term of OEIS sequence A125663. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for indices < 1 */

    /* Persistent cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* Next candidate to examine (first known term is 20). */
    static idx_t cand   = 20;

    /* Scan until we have at least n terms cached. */
    while (len < n) {
        /* Number of decimal digits of candidate. */
        int   d        = num_digits_idx(cand);   /* floor(log10(cand)) + 1 */
        int   half     = d / 2;             /* floor(d/2) */
        idx_t pow_half = pow10_idx(half);          /* 10^half */
        idx_t pow_left = pow10_idx(d - half);      /* 10^(d-half) */

        /* Extract leftmost and rightmost halves. */
        idx_t left  = cand / pow_left;          /* first 'half' digits */
        idx_t right = cand % pow_half;          /* last  'half' digits */

        /* Check the property using the prime test. */
        if (is_prime_big((seq_t)left) && !is_prime_big((seq_t)right)) {
            /* Grow cache if needed. */
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 64 : (cap << 1);
                seq_t *tmp;
                if (cap == 0)
                    tmp = seq_alloc_array(new_cap);
                else
                    tmp = seq_realloc(cache, cap, new_cap);
                if (!tmp)                /* allocation failure */
                    return 0;
                cache = tmp;
                cap   = new_cap;
            }
            /* Store the term.  The conversion (seq_t)cand is valid for
             * all supported seq_t types (integer or arbitrary‑precision). */
            cache[len++] = (seq_t)cand;
        }

        ++cand;      /* move to next integer */
    }

    return cache[n - 1];
}
