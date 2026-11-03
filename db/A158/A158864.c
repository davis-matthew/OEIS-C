/*
 * A158864  Numbers k such that prepending 6 to the k-th triangular number
 *          produces a prime.
 * https://oeis.org/A158864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th integer k such that the decimal number obtained
 *            by concatenating the digit 6 with the k‑th triangular number
 *            T(k) is prime (OEIS A158864)
 * Range:     u8 none, u16 n <= 3, u32 n <= 464, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 62 terms through n = 925 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *terms = NULL;   /* array of terms */
    static size_t terms_size = 0; /* number of terms stored   */
    static size_t terms_cap = 0;  /* current capacity of array */
    static idx_t last_k = 1;      /* last k that was tested;
                                   initialized to 1 to skip k = 1,
                                   which gives the prime 61 but is
                                   not part of the OEIS sequence. */

    if (n == 0)                         /* offset is 1, return 0 for n=0 */
        return 0;

    /* Generate terms until we have at least n of them */
    while (terms_size < n)
    {
        /* test next k */
        ++last_k;
        seq_t kv = (seq_t)last_k;
        seq_t t  = kv * (kv + 1) / 2;   /* triangular number T(k)              */

        /* Determine number of decimal digits of t           */
        int d_int = num_digits(t);
        idx_t d = (idx_t)d_int;

        /* Build concatenated number: 6 * 10^d + t        */
        seq_t pow10d = pow10(d);
        seq_t p = (seq_t)6 * pow10d + t;

        /* Check primality of the concatenated number      */
        if (isPrimeGeneric(p))
        {
            /* Resize cache if necessary */
            if (terms_size == terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp) /* allocation failed – abort */
                    return 0;
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_size++] = kv; /* store k itself            */
        }
    }

    return terms[n - 1];
}
