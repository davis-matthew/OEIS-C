/*
 * A035255  Indices of the nonzero terms in expansion of Dirichlet series
 *          Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m=
 *          11.
 * https://oeis.org/A035255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   legendre_11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A035255 – indices of non‑zero terms of
    \prod_p (1-(\kappa+1)p^{-s}+\kappa p^{-2s})^{-1},  \kappa=(11/p),
    where (a/p) is the Kronecker symbol.  The integer n is in the
    sequence iff for every prime divisor p≠11 with (11/p)=-1 the
    exponent of p in n is even. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/legendre_11.h"

/* Test whether an integer n satisfies the parity condition on its
   prime exponents. */
static bool is_good(uint64_t n)
{
    if (n == 1) return true;
    uint64_t temp = n;

    while (temp > 1) {
        uint64_t p = 0;
        /* isolate the smallest prime factor */
        if (temp % 2 == 0) {
            p = 2;
        } else {
            uint64_t d = 3;
            while (d * d <= temp && temp % d != 0) d += 2;
            p = (temp % d == 0) ? d : temp;
        }

        /* count exponent of p in n */
        unsigned cnt = 0;
        while (temp % p == 0) {
            temp /= p;
            ++cnt;
        }

        int leg = legendre_11(p);
        if (leg == -1) {                /* odd exponent forbidden */
            if (cnt & 1) return false;
        }
    }
    return true;
}

/* Cached sequence storage */
static seq_t   *seq = NULL;      /* array of sequence values */
static size_t  seq_len   = 0;    /* number of elements stored */
static size_t  seq_cap   = 0;    /* allocated capacity */
static uint64_t next_val = 1;    /* next integer to examine */

/* Ensure the array has room for at least `need` elements. */
static void ensure_capacity(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
    while (new_cap < need) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Public function required by the evaluation harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */
    while (seq_len < (size_t)n) {
        if (is_good(next_val)) {
            ensure_capacity(seq_len + 1);
            seq[seq_len++] = (seq_t)next_val;
        }
        ++next_val;
    }
    return seq[n - 1];
}
