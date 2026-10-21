/*
 * A207404  Number of 4Xn 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 54, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A207404: number of 4×n 0..1 arrays avoiding
 * horizontally 0 0 1 and 0 1 1 and vertically 0 0 1 and 1 1 0.
 *
 * Empirical recurrence (order 16) for n ≥ 17:
 *
 *   a(n) = 2*a(n-1) + 6*a(n-2) - 14*a(n-3) - 14*a(n-4)
 *          + 42*a(n-5) + 14*a(n-6) - 70*a(n-7) + 70*a(n-9)
 *          - 14*a(n-10) - 42*a(n-11) + 14*a(n-12) + 14*a(n-13)
 *          - 6*a(n-14) - 2*a(n-15) + a(n-16)
 *
 * Initial values (n = 1 … 16):
 *   8, 64, 216, 729, 1782, 4356, 8910, 18225,
 *   33210, 60516, 101598, 170569, 269276,
 *   425104, 639612, 962361
 */

#include <stdint.h>

/* Initial values for n = 1 … 16 */
static const seq_t init_vals[16] = {
    8,        /* a(1) */
    64,       /* a(2) */
    216,      /* a(3) */
    729,      /* a(4) */
    1782,     /* a(5) */
    4356,     /* a(6) */
    8910,     /* a(7) */
    18225,    /* a(8) */
    33210,    /* a(9) */
    60516,    /* a(10) */
    101598,   /* a(11) */
    170569,   /* a(12) */
    269276,   /* a(13) */
    425104,   /* a(14) */
    639612,   /* a(15) */
    962361    /* a(16) */
};

/* Coefficients for the 16‑term recurrence, indexed by offset
   from a(n-1).  coeff[0] multiplies a(n-1), coeff[15] multiplies a(n-16). */
static const int coeff[16] = {
     2,   /* a(n-1)  */
     6,   /* a(n-2)  */
    -14,  /* a(n-3)  */
    -14,  /* a(n-4)  */
     42,  /* a(n-5)  */
     14,  /* a(n-6)  */
    -70,  /* a(n-7)  */
      0,  /* a(n-8)  */
     70,  /* a(n-9)  */
    -14,  /* a(n-10) */
    -42,  /* a(n-11) */
     14,  /* a(n-12) */
     14,  /* a(n-13) */
     -6,  /* a(n-14) */
     -2,  /* a(n-15) */
     1    /* a(n-16) */
};

/*
 * Compute a(n) for any n >= 1.
 *
 * The function uses a circular buffer of size 16 holding the
 * most recent 16 values of the sequence.  For n <= 16 the
 * corresponding constant is returned directly.  For larger n
 * the recurrence is applied iteratively.
 */
seq_t a(idx_t n)
{
    if (n <= 16)
        return init_vals[n - 1];

    /* Circular buffer: buffer[i] holds a(k-16 + i),
       where k is the current n being processed. */
    seq_t buffer[16];
    for (int i = 0; i < 16; ++i)
        buffer[i] = init_vals[i];

    /* Compute terms from 17 up to n. */
    for (idx_t k = 17; k <= n; ++k) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        /* a(k-1) corresponds to buffer[15], a(k-2) to buffer[14], etc. */
        for (int i = 0; i < 16; ++i) {
            int c = coeff[i];
            seq_t term = buffer[15 - i];   /* a(k-i-1) */

            if (c > 0)
                sum_pos += (seq_t)c * term;
            else if (c < 0)
                sum_neg += (seq_t)(-c) * term;
        }

        seq_t new_val = sum_pos - sum_neg;

        /* Advance the circular buffer: drop the oldest value and store the new one. */
        for (int i = 0; i < 15; ++i)
            buffer[i] = buffer[i + 1];
        buffer[15] = new_val;
    }

    /* After the loop, buffer[15] holds a(n). */
    return buffer[15];
}
