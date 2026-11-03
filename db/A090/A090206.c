/*
 * A090206  Nonprime Fibonacci numbers.
 * https://oeis.org/A090206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = n-th nonprime Fibonacci number (including 0 and 1). */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
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

static seq_t *allocate_seq_array(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

/* Allocate an array of seq_t suitable for both C and C++ */

/* Free an array allocated by allocate_seq_array */
static void free_seq_array(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    /* Static state: cached composites and Fibonacci generator */
    static seq_t *composites = NULL;   /* array of nonprime Fibonacci numbers */
    static size_t cap     = 0;         /* allocated capacity */
    static size_t len     = 0;         /* number of composites stored */
    static seq_t f0 = 0;               /* current Fibonacci number */
    static seq_t f1 = 1;               /* next Fibonacci number */

    /* Generate Fibonacci numbers until we have at least n composites */
    while (len < n) {
        seq_t cur = f0;   /* current Fibonacci term */

        /* 0 and 1 are not prime, so include them; skip primes */
        if (!is_prime_big(cur)) {
            if (len == cap) {
                size_t new_cap = (cap == 0 ? 8 : cap * 2);
                seq_t *new_arr = allocate_seq_array(new_cap);
                for (size_t i = 0; i < len; ++i) new_arr[i] = composites[i];
                if (composites) free_seq_array(composites);
                composites = new_arr;
                cap = new_cap;
            }
            composites[len++] = cur;
        }

        /* Advance Fibonacci generator: f0, f1 = f1, f0+f1 */
        seq_t next = f0 + f1;
        f0 = f1;
        f1 = next;
    }

    return composites[n - 1];
}
