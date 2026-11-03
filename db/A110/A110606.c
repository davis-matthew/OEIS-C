/*
 * A110606  Numbers n whose base 8 representations, interpreted as base 10
 *          integers, are semiprimes.
 * https://oeis.org/A110606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n whose base‑8 representation, interpreted as a 
   decimal integer, is a semiprime.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

/* true if x is the product of exactly two primes (possibly the same) */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4)
        return false;
    uint64_t d = 0;
    if (x % 2 == 0)
        d = 2;
    else {
        for (uint64_t dd = 3; dd <= x / dd; dd += 2) {
            if (x % dd == 0) {
                d = dd;
                break;
            }
        }
    }
    if (d == 0)
        return false;                      /* x is prime */
    uint64_t second = x / d;
    return is_prime_u64(second);
}

/* Allocate an array of seq_t, compatible with C and C++ */
static inline seq_t *alloc_seq_array(size_t cap)
{
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t *)malloc(cap * sizeof(seq_t));
#endif
}

/* dynamic array helpers -------------------------------------------------- */
static seq_t *resize_array(seq_t *old, idx_t old_size, idx_t new_size)
{
    seq_t *new_arr;
#ifdef __cplusplus
    new_arr = new seq_t[new_size]();        /* value–init for C++ objects   */
#else
    new_arr = (seq_t *)calloc(new_size, sizeof(seq_t));   /* zero for C */
#endif
    /* copy old data */
    idx_t i;
    for (i = 0; i < old_size && i < new_size; ++i)
        new_arr[i] = old[i];

    /* free old storage */
    if (old) {
#ifdef __cplusplus
        delete[] old;
#else
        free(old);
#endif
    }
    return new_arr;
}

/* Convert an integer n into the decimal number obtained by reading
   the base‑8 digits of n as decimal digits. */
static inline seq_t base8_to_decimal(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    /* store octal digits in least‑significant‑first order */
    uint8_t digits[32]; /* sufficient for 64‑bit integers */
    size_t cnt = 0;
    while (n > 0) {
        digits[cnt++] = n & 7;     /* n % 8 */
        n >>= 3;                  /* n /= 8 */
    }
    /* build the decimal value from most‑significant digit */
    seq_t val = 0;
    for (size_t i = cnt; i > 0; --i) {
        val = val * 10 + digits[i - 1];
    }
    return val;
}

/* Return the n‑th term of the sequence.  */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for index 0; placeholder */
        return (seq_t)0;

    /* memoisation storage */
    static seq_t *terms      = NULL;
    static size_t terms_cap  = 0;
    static idx_t terms_size  = 0;
    static idx_t last_candidate = 1;   /* next integer to test */

    /* grow the storage if necessary */
    if (terms_size < n) {
        if (terms_cap < (size_t)n) {
            size_t new_cap = terms_cap ? terms_cap * 2 : 16;
            while (new_cap < (size_t)n)
                new_cap <<= 1;
            if (terms == NULL)
                terms = alloc_seq_array(new_cap);
            else
                terms = resize_array(terms, terms_cap, new_cap);
            terms_cap = new_cap;
        }

        /* generate terms until we have n of them */
        while (terms_size < n) {
            for (;;) {
                seq_t f = base8_to_decimal(last_candidate);
                if (is_semiprime_uint64((uint64_t)f)) {
                    terms[terms_size++] = last_candidate;
                    break;
                }
                ++last_candidate;   /* next integer to test */
            }
            ++last_candidate;       /* start next search at next integer */
        }
    }
    return terms[n - 1];
}
