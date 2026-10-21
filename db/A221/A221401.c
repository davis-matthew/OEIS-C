/*
 * A221401  Number of 2 X n arrays of occupancy after each element moves to
 *          some horizontal, diagonal or antidiagonal neighbor, with no
 *          2-loops and with no occupancy greater than 2.
 * https://oeis.org/A221401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 6*a(n-2) - 74*a(n-3) + 8*a(n-4) + 132*a(n-5) -
 *            72*a(n-6) - 24*a(n-7) + 16*a(n-8), with a(1)=0, a(2)=1, a(3)=6,
 *            a(4)=56, a(5)=324, a(6)=2196, a(7)=13328, a(8)=83680
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for n = 1 … 8 */
    static const seq_t base[9] = {
        0,
        0,    /* a(1) */
        1,    /* a(2) */
        6,    /* a(3) */
        56,   /* a(4) */
        324,  /* a(5) */
        2196, /* a(6) */
        13328,/* a(7) */
        83680 /* a(8) */
    };

    /* Return the base value for n <= 8 */
    if (n <= 8) {
        return base[n];
    }

    /* Circular buffer holding the last 8 terms:
       buffer[0] = a(n-7), … , buffer[7] = a(n-1) during the iteration.
    */
    seq_t buffer[8];
    for (idx_t i = 0; i < 8; ++i) {
        buffer[i] = base[i + 1];
    }

    /* Compute terms from a(9) up to a(n) */
    for (idx_t cur = 9; cur <= n; ++cur) {
        /* Compute positive and negative contributions separately to avoid
           unsigned underflow.  Each term is non‑negative. */
        seq_t pos = 0;
        pos += buffer[7] * (seq_t)7;   /* 7*a(n-1) */
        pos += buffer[6] * (seq_t)6;   /* 6*a(n-2) */
        pos += buffer[4] * (seq_t)8;   /* 8*a(n-4) */
        pos += buffer[3] * (seq_t)132; /* 132*a(n-5) */
        pos += buffer[0] * (seq_t)16;  /* 16*a(n-8) */

        seq_t neg = 0;
        neg += buffer[5] * (seq_t)74;  /* 74*a(n-3) */
        neg += buffer[2] * (seq_t)72;  /* 72*a(n-6) */
        neg += buffer[1] * (seq_t)24;  /* 24*a(n-7) */

        /* Next term */
        seq_t next = pos - neg;

        /* Shift the window one position to the left */
        for (idx_t i = 0; i < 7; ++i) {
            buffer[i] = buffer[i + 1];
        }
        buffer[7] = next;
    }

    return buffer[7];
}
