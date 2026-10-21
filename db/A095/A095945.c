/*
 * A095945  a(n) = n + number of letters in the English name of n.
 * https://oeis.org/A095945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = n + number of letters in the English name of n (British
 *            style: includes "and" for numbers above 100 with a non‑zero
 *            remainder) The English names are written in short scale
 *            (thousand, million, ...)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Letter counts for units and teens 0‑19 (0 unused except for 0 itself) */
static const int unit_len[20] = {
    0,  /* 0 (unused, see special case below) */
    3,  /* one */
    3,  /* two */
    5,  /* three */
    4,  /* four */
    4,  /* five */
    3,  /* six */
    5,  /* seven */
    5,  /* eight */
    4,  /* nine */
    3,  /* ten */
    6,  /* eleven */
    6,  /* twelve */
    8,  /* thirteen */
    8,  /* fourteen */
    7,  /* fifteen */
    7,  /* sixteen */
    9,  /* seventeen */
    8,  /* eighteen */
    8   /* nineteen */
};

/* Letter counts for tens 20‑90 */
static const int tens_len[10] = {
    0,0,6,6,5,5,5,7,6,6  /* index 2..9 used */
};

/* Letter counts for scale words */
static const int scale_len[7] = {
    0,   /* units */
    8,   /* thousand */
    7,   /* million */
    7,   /* billion */
    8,   /* trillion */
    11,  /* quadrillion */
    10   /* quintillion */
};

/* Count letters in a 3‑digit chunk (1–999).  The input must be >0. */
static int word3_digit_len(int n)
{
    int h = n / 100;
    int rem = n % 100;
    int len = 0;

    if (h > 0) {
        len += unit_len[h];   /* e.g., "one" */
        len += 7;             /* "hundred" */
    }
    if (rem > 0) {
        if (h > 0) len += 3;  /* "and" */
        if (rem < 20) {
            len += unit_len[rem];
        } else {
            int t = rem / 10;
            int o = rem % 10;
            len += tens_len[t];
            if (o > 0) len += unit_len[o];
        }
    }
    return len;
}

/* Count letters in the English name of n (0 ≥ n). */
static int english_len_u64(uint64_t n)
{
    if (n == 0) return 4;   /* "zero" */

    int total = 0;
    int scale = 0;

    while (n > 0) {
        int chunk = (int)(n % 1000);
        if (chunk > 0) {
            total += word3_digit_len(chunk);
            if (scale > 0) total += scale_len[scale];
        }
        n /= 1000;
        scale++;
    }
    return total;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    int add = english_len_u64((uint64_t)n);
    return val + (seq_t)add;
}
