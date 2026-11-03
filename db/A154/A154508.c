/*
 * A154508  Numbers k such that appending k to the k-th nonprime yields a
 *          prime.
 * https://oeis.org/A154508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154508: k such that concatenation of the k‑th nonprime and k is prime.
   a(n) returns the n‑th such k (offset 1). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/num_digits.h"
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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* below offset – behaviour irrelevant */

    /* --------- cache for sequence values --------- */
    static seq_t *seq_cache = NULL;
    static size_t seq_cap  = 0;
    static size_t seq_len  = 0;

    if (seq_cache == NULL)
    {
        seq_cache = seq_alloc(n);
        seq_cap   = n;
        seq_len   = 0;
    }
    else if (seq_cap < n)
    {
        seq_cache = seq_realloc(seq_cache, seq_cap, n);
        seq_cap   = n;
    }

    /* --------- cache for non‑prime numbers --------- */
    static seq_t *nonprime_cache = NULL;
    static size_t nonprime_cap   = 0;
    static size_t nonprime_len   = 0;
    static uint64_t cur_val      = 0;   /* next integer to test for non‑prime */

    /* --------- k values to examine --------- */
    static idx_t next_k = 1;

    /* Generate sequence values until we have n of them */
    while (seq_len < n)
    {
        /* Make sure we have the next_k‑th nonprime available */
        while (nonprime_len < next_k)
        {
            /* Grow the nonprime cache if necessary */
            if (nonprime_cap == 0)
            {
                nonprime_cap   = 16;
                nonprime_cache = seq_alloc(nonprime_cap);
            }
            else if (nonprime_len == nonprime_cap)
            {
                size_t new_cap   = nonprime_cap * 2;
                nonprime_cache   = seq_realloc(nonprime_cache, nonprime_cap, new_cap);
                nonprime_cap     = new_cap;
            }

            /* Find the next nonprime */
            while (is_prime_seq((seq_t)cur_val))
                cur_val++;                 /* skip primes */

            nonprime_cache[nonprime_len++] = cur_val;
            cur_val++;                     /* advance for next candidate */
        }

        /* Retrieve the next_k‑th nonprime */
        seq_t np = nonprime_cache[next_k - 1];

        /* Compute concatenation np || next_k */
        int dk = num_digits((seq_t)next_k);                 /* number of decimal digits of next_k */
        seq_t pow10d = pow10((idx_t)dk);
        seq_t concat  = np * pow10d + (seq_t)next_k;

        /* Test primality of the concatenated number */
        if (is_prime_big(concat))
            seq_cache[seq_len++] = next_k;

        ++next_k;
    }

    return seq_cache[n - 1];
}
