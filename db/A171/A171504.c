/*
 * A171504  a(n) = n-th nonprime + n.
 * https://oeis.org/A171504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth nonprime + n
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

/* Compute the sequence A171504: a(n) = (n-th nonprime) + n
   Nonprimes include 1, 4, 6, 8, 9, 10, …   */
seq_t a(idx_t n)
{
    /* Static cache of known nonprimes */
    static seq_t *nonprimes = NULL;   /* array holding the nonprimes found so far */
    static size_t nc_len   = 0;       /* number of elements currently stored */
    static size_t nc_cap   = 0;       /* allocated capacity of the array */

    /* Ensure the cache contains at least n entries.  If it doesn't,
       extend the sequence by testing successive integers until the
       required count is reached. */
    if (nc_len < n) {
        /* Grow the array if necessary before adding new entries. */
        if (nc_cap == 0) {
            /* Initial capacity of 8 should cover early growth. */
            nc_cap = 8;
            nonprimes = seq_realloc(NULL, 0, nc_cap);
        } else if (nc_len == nc_cap) {
            nc_cap <<= 1; /* double the capacity */
            nonprimes = seq_realloc(nonprimes, nc_cap >> 1, nc_cap);
        }

        /* Start from value 1 if we have no entries yet, otherwise
           from the next integer after the last stored nonprime. */
        seq_t cur = (nc_len == 0) ? 1 : nonprimes[nc_len - 1] + 1;

        while (nc_len < n) {
            /* Add to cache if cur is not a prime (i.e., is a nonprime). */
            if (!is_prime_big(cur)) {
                nonprimes[nc_len] = cur;
                ++nc_len;
            }
            /* Next candidate.  The addition is safe for arbitrary‑precision
               seq_t; it simply increments by one. */
            cur = cur + 1;
        }
    }

    /* Compute a(n) = (n‑th nonprime) + n.
       Convert n to seq_t for the addition. */
    seq_t result = nonprimes[n - 1] + (seq_t)n;
    return result;
}
