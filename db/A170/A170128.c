/*
 * A170128  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of reduced words of length n in the Coxeter *
 *            group with 23 generators S_i satisfying S_i^2 = 1 and * (S_i
 *            S_j)^38 = 1 (i ≠ j). For n < 38, a(n) = 23·22^(n-1) (with
 *            a(0)=1). * For n ≥ 38, the generating function reduces to the
 *            linear recurrence * 11·a(n) = 11·a(n-38) − Σ_{k=1}^{37}...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/*  seq_t  and  idx_t are defined by the build system.  We do not
 *  assume any particular size – the code works for 8, 16, 32, 64
 *  or 128‑bit integers as well as arbitrary precision integers. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* buffer to hold the last 38 terms of the sequence */
    seq_t buf[38];
    /* compute first 38 terms */
    buf[0] = (seq_t)1;
    for (idx_t i = 1; i <= 37; ++i) {
        /* a(i) = 23 * 22^(i-1) */
        seq_t val = (seq_t)23;
        for (idx_t j = 1; j < i; ++j) {
            val *= (seq_t)22;
        }
        buf[i] = val;
    }

    if (n <= 37) {
        return buf[n];
    }

    /* iterate from 38 up to n */
    for (idx_t cur = 38; cur <= n; ++cur) {
        /* sum of the previous 37 terms a(cur-1) ... a(cur-37) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= 37; ++k) {
            sum += buf[(cur - k) & 37];   /* (cur - k) % 38  */
        }
        seq_t prev_big = buf[(cur - 38) & 37];
        seq_t numerator = (seq_t)11 * prev_big - sum;
        /* division by 11 is guaranteed to be exact */
        seq_t next = numerator / (seq_t)11;
        buf[cur & 37] = next;  /* store into the circular buffer */
    }

    return buf[n & 37];
}
