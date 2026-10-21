/*
 * A204282  Number of (n+2) X 8 0..2 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically, diagonally
 *          or antidiagonally exactly three ways, and new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A204282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-4) + 4*a(n-6) (n > 8) a(1)=14141,
 *            a(2)=735, a(3)=778, a(4)=982, a(5)=996, a(6)=1496, a(7)=2860,
 *            a(8)=5500
 * Range:     u8 none, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
using std::vector;
#endif

/* Initial values for n = 1 .. 8 (index 0 unused) */
static const seq_t init_vals[9] = {
    0,          /* dummy 0th element */
    14141,      /* a(1) */
    735,        /* a(2) */
    778,        /* a(3) */
    982,        /* a(4) */
    996,        /* a(5) */
    1496,       /* a(6) */
    2860,       /* a(7) */
    5500        /* a(8) */
};

seq_t a(idx_t n) {
    /* Handle small n directly */
    if (n >= 1 && n <= 8) {
        return init_vals[(size_t)n];
    }

    /* Allocate a buffer to compute the recurrence from 1 .. n.
       We use std::vector in C++ to get proper construction / destruction
       for arbitrary-precision types; in C we use malloc. */
    #ifdef __cplusplus
    vector<seq_t> buf((size_t)n + 1);
    #else
    seq_t *buf = (seq_t*)malloc(((size_t)n + 1) * sizeof(seq_t));
    if (!buf) {
        /* allocation failed – return zero as a safe fallback */
        return (seq_t)0;
    }
    #endif

    /* Copy the known initial terms */
    for (size_t i = 1; i <= 8; ++i) {
        buf[i] = init_vals[i];
    }

    /* Compute the sequence by recurrence */
    for (size_t i = 9; i <= (size_t)n; ++i) {
        /* a(i) = 3*a(i-1) - 6*a(i-4) + 4*a(i-6) */
        seq_t val = 3 * buf[i - 1];
        val -= 6 * buf[i - 4];
        val += 4 * buf[i - 6];
        buf[i] = val;
    }

    /* Result */
    seq_t result = buf[(size_t)n];

    /* Free memory if we used malloc */
    #ifndef __cplusplus
    free(buf);
    #endif

    return result;
}
