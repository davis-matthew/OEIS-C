/*
 * A216440  a(n) = smallest m such that 2n-1 | 2^m+1, or 0 if no such m
 *          exists.
 * https://oeis.org/A216440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 260, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A216440: a(n) = smallest m such that 2n-1 | 2^m+1, or 0 if none */
#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n=0, but return 0 */
    if (n == 1)           /* 2*1-1 = 1, 2^1+1 is divisible by 1 */
        return 1;

    /* modulus d = 2*n - 1 */
    seq_t d = 2 * n - 1;

    /* compute φ(d) */
    idx_t temp = (idx_t)d;
    seq_t phi = d;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            /* p is a prime factor of d */
            while (temp % p == 0) temp /= p;
            phi -= phi / p;
        }
    }
    if (temp > 1)          /* remaining prime factor */
        phi -= phi / temp;

    /* find multiplicative order of 2 modulo d */
    seq_t ord = phi;

    /* factor ord to reduce it */
    idx_t t = (idx_t)ord;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            /* try dividing out p as many times as possible */
            while (ord % p == 0) {
                seq_t new_ord = ord / p;
                if (mod_pow((seq_t)2, (uint64_t)new_ord, d) == 1)
                    ord = new_ord;
                else
                    break;
            }
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) {                /* remaining prime factor of ord */
        idx_t p = t;
        while (ord % p == 0) {
            seq_t new_ord = ord / p;
            if (mod_pow((seq_t)2, (uint64_t)new_ord, d) == 1)
                ord = new_ord;
            else
                break;
        }
    }

    /* check for a solution m = ord/2 with 2^m ≡ -1 (mod d) */
    if ((ord & 1) == 0) {          /* ord even */
        seq_t half = ord >> 1;
        if (mod_pow((seq_t)2, (uint64_t)half, d) == d - 1)
            return half;
    }

    return 0;   /* no such m exists */
}
