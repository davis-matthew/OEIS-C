/*
 * A060704  Singular n X n matrices over GF(2).
 * https://oeis.org/A060704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2^(n^2) – ∏_{i=0}^{n-1} (2^n – 2^i)
 * for n ≥ 1.  The function uses a guarded left shift
 * so that for fixed‑width integer types the shift count
 * never exceeds the bit width, avoiding undefined behaviour.
 * For arbitrary‑precision integer types the shift is performed
 * normally since the overloaded operator supports arbitrary
 * shift amounts.
 */

#include <stddef.h>
#include <stdint.h>
#ifndef CHAR_BIT
# define CHAR_BIT 8
#endif

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n = 0, return 0 as a safe default */
        return (seq_t)0;

    /* total = 2^(n^2), guarded against shifting by ≥ bit width */
    seq_t total;
#if (__STDC_VERSION__ >= 199901L)  /* C99 and later may have __builtin_clz etc. */
    /* Check if seq_t is a built‑in unsigned integer type by its size */
    if (sizeof(seq_t) <= 16) {                /* uint64_t or unsigned __int128 */
        /* Compute the shift count in a 64‑bit integer */
        idx_t shift = n * n;
        /* Number of bits in the type */
        idx_t bits = (idx_t)(sizeof(seq_t) * CHAR_BIT);
        if (shift >= bits)
            total = (seq_t)0;                 /* 2^bits ≡ 0 (mod 2^bits) */
        else
            total = ((seq_t)1) << shift;
    } else {
        /* For arbitrary‑precision types trusts the overloaded shift operator */
        total = (((seq_t)1) << ((idx_t)n * n));
    }
#else
    /* Pre‑C99 compilers: assume all seq_t are arbitrary‑precision */
    total = (((seq_t)1) << ((idx_t)n * n));
#endif

    /* 2^n */
    seq_t pow2n;
    {
#if (__STDC_VERSION__ >= 199901L)
        if (sizeof(seq_t) <= 16) {
            idx_t bits = (idx_t)(sizeof(seq_t) * CHAR_BIT);
            if (n >= bits)
                pow2n = (seq_t)0;          /* 2^bits ≡ 0 mod 2^bits */
            else
                pow2n = ((seq_t)1) << n;
        } else {
            pow2n = (((seq_t)1) << n);
        }
#else
        pow2n = (((seq_t)1) << n);
#endif
    }

    /* product_{i=0}^{n-1} (2^n – 2^i) */
    seq_t prod = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        seq_t term = pow2n - (((seq_t)1) << i);
        prod *= term;
    }

    /* a(n) = total - prod */
    return total - prod;
}
