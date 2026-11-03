/*
 * A056902  Numbers n where 36n^2+24n+7 is prime (sorted by absolute values
 *          with negatives before positives).
 * https://oeis.org/A056902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 28, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 60 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A056902: numbers m such that 36*m^2+24*m+7 is prime.
   The terms are ordered by increasing absolute value, with negative
   terms listed before their positive counterparts.  */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

/* Return the n-th member of the sequence (n ≥ 0). */
seq_t a(idx_t n)
{
    /* Cache of already-computed terms. */
    static seq_t *terms = NULL;
    static size_t len   = 0;   /* number of terms stored */
    static size_t cap   = 0;   /* allocated capacity */
    static idx_t  next_k = 0;  /* next absolute value to test */
    static int    next_sign = 0; /* 0 → negative, 1 → positive */
    static bool   init = false;

    /* Initialisation: 0 is always a term. */
    if (!init) {
        cap = 16;
        terms = mem_new(seq_t, cap);
        terms[0] = (seq_t)0;
        len    = 1;
        next_k = 1;
        next_sign = 0;   /* start with the negative of 1 */
        init = true;
    }

    /* Generate terms until we have at least n+1 of them. */
    while (len <= n) {
        seq_t m;
        /* Pick next candidate: negative then positive for a given |m|. */
        if (next_sign == 0) {          /* negative side */
            m = -(seq_t)next_k;
            next_sign = 1;
        } else {                       /* positive side */
            m = (seq_t)next_k;
            next_sign = 0;
            next_k++;                  /* advance to next absolute value */
        }

        /* Compute test value: 36*m^2 + 24*m + 7.  Use a wider type if
           possible to avoid overflow of the underlying seq_t. */
        seq_t expr;
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
        /* If sizeof(seq_t) fits within 64‑bit, use 128‑bit arithmetic. */
        if (sizeof(seq_t) <= 8) {
            /* m is finite in this case; cast to a signed 64‑bit int. */
            __int128 mm = (__int128)((long long)m);
            __int128 v128 = 36 * mm * mm + 24 * mm + 7;
            expr = (seq_t)v128;               /* implicit conversion to seq_t */
        } else
#endif
        {
            /* For arbitrary‑precision seq_t, rely on its own operators. */
            expr = ((seq_t)36 * m * m) + ((seq_t)24 * m) + (seq_t)7;
        }

        /* If the value is prime, store the corresponding m. */
        if (is_prime_big(expr)) {
            if (len == cap) {
                cap *= 2;
                terms = mem_resize(seq_t, terms, cap);
            }
            terms[len++] = m;
        }
    }

    return terms[n];
}
