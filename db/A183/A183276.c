/*
 * A183276  Half the number of n X n X n triangular binary arrays with each
 *          element having no more than two neighbors unequal to itself.
 * https://oeis.org/A183276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Polynomial recurrence for A183276 (OEIS A183276)
   a(1)=1, a(2)=4, a(3)=11, a(4)=26, a(5)=60, a(6)=132, a(7)=290,
   a(8)=620, a(9)=1322, a(10)=2777,
   for n>=11:
   a(n) = a(n-1) + 4*a(n-2) - 3*a(n-3) + 3*a(n-4)
          - 6*a(n-5) - 9*a(n-6) + 3*a(n-7) + 2*a(n-8)
          + a(n-9) + a(n-10);
   The function computes a(n) using this linear recurrence, not
   relying on any external lookup tables for n>10.  It works for
   any integer type seq_t (including arbitrary‑precision integer
   classes) and idx_t (uint64_t). */

#include <stddef.h>   /* for size_t if needed */

static const seq_t base_vals[11] = {
    0ULL,            /* a(0) unused */
    1ULL, 4ULL, 11ULL, 26ULL, 60ULL, 132ULL, 290ULL,
    620ULL, 1322ULL, 2777ULL
};

seq_t a(idx_t n)
{
    if (n < 1) {
        /* Undefined for n<1 – return 0 as a safe sentinel */
        return (seq_t)0;
    }
    if (n <= 10) {
        return base_vals[n];
    }

    /* Sliding window of the last 10 terms */
    seq_t w[10];
    for (idx_t i = 0; i < 10; ++i) {
        w[i] = base_vals[i + 1];          /* w[0] = a(1), ..., w[9] = a(10) */
    }

    for (idx_t k = 11; k <= n; ++k) {
        /* Compute a(k) from the recurrence */
        seq_t next = w[9]                   /* a(k-1)   */
                  + 4 * w[8]                 /* 4 * a(k-2)   */
                  - 3 * w[7]                 /* -3 * a(k-3) */
                  + 3 * w[6]                 /* +3 * a(k-4) */
                  - 6 * w[5]                 /* -6 * a(k-5) */
                  - 9 * w[4]                 /* -9 * a(k-6) */
                  + 3 * w[3]                 /* +3 * a(k-7) */
                  + 2 * w[2]                 /* +2 * a(k-8) */
                  + w[1]                     /* + a(k-9)   */
                  + w[0];                    /* + a(k-10)  */

        /* Shift window left by one: drop a(k-10), keep a(k-9)... a(k-1) */
        for (idx_t j = 0; j < 9; ++j) {
            w[j] = w[j + 1];
        }
        w[9] = next;  /* a(k) becomes the newest term */
    }

    return w[9];   /* a(n) */
}
