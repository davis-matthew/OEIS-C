/*
 * A203575  Array of certain four complete residue classes (nonnegative
 *          members), read by SW-NE antidiagonals.
 * https://oeis.org/A203575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) is the nth element of the array obtained by * filling a
 *            4‑column array with the four complete residue classes * C0 =
 *            {0,4,8,12,…} * C1 = {1,7,9,15,17,23,…} * C2 =
 *            {2,6,10,14,18,22,…} * C3 = {3,5,11,13,19,21,…} * then reading
 *            that array by southwest‑to‑northeast antidiagonals
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t next_class_value(int cls, idx_t cnt)
{
    switch (cls) {
        case 0: /* class 0: 0,4,8,12,… */
            return (seq_t)4 * cnt;
        case 1: /* class 1: 1,7,9,15,17,23,… */
            if ((cnt & 1) == 0)
                return (seq_t)1 + (seq_t)8 * (cnt >> 1);
            else
                return (seq_t)7 + (seq_t)8 * (cnt >> 1);
        case 2: /* class 2: 2,6,10,14,18,22,… */
            if ((cnt & 1) == 0)
                return (seq_t)2 + (seq_t)8 * (cnt >> 1);
            else
                return (seq_t)6 + (seq_t)8 * (cnt >> 1);
        case 3: /* class 3: 3,5,11,13,19,21,… */
            if ((cnt & 1) == 0)
                return (seq_t)3 + (seq_t)8 * (cnt >> 1);
            else
                return (seq_t)5 + (seq_t)8 * (cnt >> 1);
        default:
            return 0; /* never reached */
    }
}

seq_t a(idx_t n)
{
    /* Counters for how many times each class has already been used. */
    idx_t used[4] = {0, 0, 0, 0};

    /* Mapping for the first few antidiagonals. */
    static const int map0[1] = {0};
    static const int map1[1] = {1};
    static const int map2[2] = {0, 2};
    static const int map3[2] = {1, 0};
    static const int map4[3] = {3, 2, 1};
    static const int map5[3] = {0, 3, 2};
    static const int map6[4] = {1, 0, 3, 2};

    idx_t pos = 0;        /* how many terms have been generated */
    idx_t diag = 0;       /* current antidiagonal index (d) */

    for (;;) {
        const int *map;
        int len;

        /* Determine the mapping and length for this diagonal. */
        if (diag == 0) { len = 1; map = map0; }
        else if (diag == 1) { len = 1; map = map1; }
        else if (diag == 2) { len = 2; map = map2; }
        else if (diag == 3) { len = 2; map = map3; }
        else if (diag == 4) { len = 3; map = map4; }
        else if (diag == 5) { len = 3; map = map5; }
        else { len = 4; map = map6; }

        /* Generate the terms for this diagonal. */
        for (int c = 0; c < len; ++c) {
            int cls = map[c];
            seq_t val = next_class_value(cls, used[cls]);
            used[cls]++;

            ++pos;
            if (pos == n)
                return val;
        }
        ++diag;
    }
}
