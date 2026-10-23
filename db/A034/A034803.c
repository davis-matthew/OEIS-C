/*
 * A034803  Consider the sequence of 4-tuples {0,a,b,c} (c>=a+b; a,b,c>0)
 *          which have the smallest integer 'c' required to reach {k,k,k,k} in
 *          n steps under map {r,s,t,u}->{|r-s|,|s-t|,|t-u|,|u-r|}. This
 *          sequence gives the second term 'a' of these quadruples.
 * https://oeis.org/A034803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Trib(2*q-3)+Trib(2*q-1) if r=0; Trib(2*q-2)+Trib(2*q-1)
 *            if r=1; Trib(2*q) if r=2; where q = floor((n-1)/3), r = (n-1)
 *            mod 3, and Trib is the Tribonacci sequence A000073 with
 *            Trib(-3)=0, Trib(-2)=−1, Trib(-1)=1. The implementation
 *            explicitly handles the few cases where negative indices woul...
 * Range:     u8 n <= 20, u16 n <= 33, u32 n <= 60, u64 n <= 115,
 *            u128 n <= 225, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* memoised Tribonacci array (indices >=0) */
static seq_t *trib = NULL;
static size_t trib_len = 0;
static size_t trib_cap = 0;

/* Ensure trib[0..idx] are defined (idx >= 0). */
static void ensure_trib(size_t idx)
{
    if (idx < trib_len) return;

    /* Allocate or enlarge the array. */
    size_t need = idx + 1;
    if (need > trib_cap) {
        size_t new_cap = trib_cap ? trib_cap * 2 : 8;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(trib, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        trib = tmp;
        trib_cap = new_cap;
    }

    /* Initialise the first three Tribonacci values once. */
    if (trib_len == 0) {
        trib[0] = (seq_t)0;
        trib[1] = (seq_t)0;
        trib[2] = (seq_t)1;
        trib_len = 3;
    }

    /* Compute missing Tribonacci entries. */
    for (size_t i = trib_len; i <= idx; ++i) {
        trib[i] = trib[i-1] + trib[i-2] + trib[i-3];
    }
    trib_len = need;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined but harmless */

    idx_t q = (n - 1) / 3;
    idx_t r = (n - 1) % 3;

    /* Handle the few cases that would need negative Tribonacci
       indices explicitly.  All other indices are non‑negative. */
    if (q == 0) {
        switch (r) {       /* Trib(-3)+Trib(-1), Trib(-2)+Trib(-1), Trib(0) */
            case 0: return (seq_t)1;
            case 1: return (seq_t)0;
            case 2: return (seq_t)0;
        }
    }
    if (q == 1) {
        switch (r) {       /* Trib(-1)+Trib(1), Trib(0)+Trib(1), Trib(2) */
            case 0: return (seq_t)1;
            case 1: return (seq_t)0;
            case 2: return (seq_t)1;
        }
    }

    size_t base = (size_t)(2 * q);          /* highest needed Trib index */
    ensure_trib(base);                      /* ensure trib[0..base] exist */

    if (r == 0) {                           /* Trib(base-3)+Trib(base-1) */
        return trib[base - 3] + trib[base - 1];
    } else if (r == 1) {                    /* Trib(base-2)+Trib(base-1) */
        return trib[base - 2] + trib[base - 1];
    } else {                                /* r==2: Trib(base) */
        return trib[base];
    }
}
