/*
 * A205441  Least h such that n divides the h-th difference between distinct
 *          odd-indexed Fibonacci numbers; the differences are ordered as in
 *          A205371.
 * https://oeis.org/A205441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) is the least index k of the difference D_k between distinct odd-indexed
 * Fibonacci numbers (ordered by increasing larger index and then smaller index)
 * that is divisible by n.  The value is obtained by detecting the first
 * repeated residue modulo n in the sequence F_{2j+1} mod n and computing
 * k = j*(j-1)/2 + i + 1 where i is the earlier occurrence of the residue.
 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not used, but defensive */
    /* allocate array for first occurrence of each residue */
    size_t sz = (size_t)n;
    int *first = (int *)malloc(sz * sizeof(int));
    if (!first) exit(EXIT_FAILURE);
    for (size_t k = 0; k < sz; ++k) first[k] = -1;

    /* Fibonacci variables modulo n */
    idx_t mod = n;
    idx_t f_prev = 0 % mod;          /* F0 mod n */
    idx_t f_curr = 1 % mod;          /* F1 mod n */

    size_t odd_index = 0;            /* j in the description, counts odd indices 0,1,2,... */
    idx_t fib_idx = 1;               /* current Fibonacci index (starts at 1 for F1) */

    while (1) {
        if (fib_idx & 1ULL) {        /* odd Fibonacci index */
            int residue = (int)f_curr;  /* f_curr < n, safe cast */
            if (first[residue] == -1) {
                first[residue] = (int)odd_index;
            } else {
                size_t i_occ = (size_t)first[residue];
                seq_t k = (seq_t)odd_index * (odd_index - 1) / 2 + (seq_t)i_occ + 1;
                free(first);
                return k;
            }
            ++odd_index;
        }

        /* next Fibonacci number modulo n */
        idx_t f_next = (f_prev + f_curr) % mod;
        f_prev = f_curr;
        f_curr = f_next;
        ++fib_idx;
    }
}
