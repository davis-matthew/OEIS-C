/*
 * A163602  First differences of A161753.
 * https://oeis.org/A163602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k_n^2 - k_{n-1}^2, where k_n is the nth nonprime
 *            (non‑prime list includes 0 and 1 starting with index 0)
 * Range:     u8 n <= 48, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   isPrimeLocal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeLocal.h"

seq_t a(idx_t n) {
    /* Static table of non‑prime numbers, indexed from 0. */
    static idx_t *nonprimes = NULL;
    static idx_t len = 0, cap = 0;
    static idx_t last_checked = 0;

    /* First‑time initialisation. */
    if (!nonprimes) {
        cap = 16;
        nonprimes = (idx_t *)malloc(cap * sizeof(idx_t));
        if (!nonprimes) abort();
        /* 0 and 1 are the first two non‑primes. */
        nonprimes[0] = 0;
        nonprimes[1] = 1;
        len = 2;
        last_checked = 1;
    }

    /* Grow the table until it contains indices 0 … n.  We need k_n and k_{n-1}. */
    while (len <= n) {
        ++last_checked;                       /* next candidate */
        if (!isPrimeLocal(last_checked)) {     /* it is a non‑prime */
            if (len == cap) {                 /* grow the array if needed */
                cap <<= 1;
                idx_t *tmp = (idx_t *)realloc(nonprimes, cap * sizeof(idx_t));
                if (!tmp) abort();
                nonprimes = tmp;
            }
            nonprimes[len++] = last_checked;
        }
    }

    /* Retrieve the needed non‑prime numbers. */
    idx_t k_prev = nonprimes[n - 1];
    idx_t k_cur  = nonprimes[n];

    /* Compute the difference of squares in seq_t type. */
    seq_t sq_prev = (seq_t)k_prev * (seq_t)k_prev;
    seq_t sq_cur  = (seq_t)k_cur  * (seq_t)k_cur;
    return sq_cur - sq_prev;
}
