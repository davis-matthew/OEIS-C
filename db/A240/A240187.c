/*
 * A240187  Number of n X 2 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or one plus the sum of elements above
 *          it or three plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A240187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A240187:
   a(n) = number of n×2 arrays with entries 0..3 such that,
          no element equals
          (0 + sum of elements to its left) mod 4  or
          (1 + sum of elements above it) mod 4  or
          (3 + sum of elements diagonally to its northwest) mod 4.
   Recurrence (empirical):
      a(n) = 2*a(n-2) + 10*a(n-3) - a(n-4) - 5*a(n-5) - 15*a(n-6)
             + a(n-7) + 4*a(n-8) + 2*a(n-9) + 10*a(n-10) + 5*a(n-11)
             - 6*a(n-13).   (n ≥ 14)
   Initial values (offset 1):
      a(1)=1,a(2)=5,a(3)=12,a(4)=27,a(5)=73,a(6)=154,
      a(7)=358,a(8)=872,a(9)=1871,a(10)=4438,a(11)=10338,
      a(12)=22880,a(13)=54100. */

seq_t a(idx_t n)
{
    /* Handle indices below 1 (offset 1).  Behavior arbitrary. */
    if (n < 1)
        return 0;

    /* Return known initial values directly. */
    static const seq_t init[14] = {
        0,          /* unused index 0 */
        1,          /* a(1) */
        5,          /* a(2) */
        12,         /* a(3) */
        27,         /* a(4) */
        73,         /* a(5) */
        154,        /* a(6) */
        358,        /* a(7) */
        872,        /* a(8) */
        1871,       /* a(9) */
        4438,       /* a(10) */
        10338,      /* a(11) */
        22880,      /* a(12) */
        54100       /* a(13) */
    };

    if (n <= 13)
        return init[n];

    /* Compute a(n) using the recurrence.  Only the last
       13 values are needed at each step. */
    seq_t buf[13];
    /* initialise buffer with a(1) .. a(13) */
    for (idx_t i = 0; i < 13; ++i)
        buf[i] = init[i + 1];

    /* buf[0] = a(i-13), buf[1] = a(i-12), ..., buf[12] = a(i-1) */

    for (idx_t k = 14; k <= n; ++k) {
        seq_t val = 0;

        /* 2*a(k-2) */
        val += 2 * buf[11];

        /* 10*a(k-3) */
        val += 10 * buf[10];

        /* - a(k-4) */
        val -= buf[9];

        /* - 5*a(k-5) */
        val -= 5 * buf[8];

        /* - 15*a(k-6) */
        val -= 15 * buf[7];

        /* + a(k-7) */
        val += buf[6];

        /* + 4*a(k-8) */
        val += 4 * buf[5];

        /* + 2*a(k-9) */
        val += 2 * buf[4];

        /* + 10*a(k-10) */
        val += 10 * buf[3];

        /* + 5*a(k-11) */
        val += 5 * buf[2];

        /* - 6*a(k-13) */
        val -= 6 * buf[0];

        /* Shift buffer left by one position */
        for (idx_t i = 0; i < 12; ++i)
            buf[i] = buf[i + 1];
        buf[12] = val;
    }

    /* After loop, buf[12] holds a(n) */
    return buf[12];
}
