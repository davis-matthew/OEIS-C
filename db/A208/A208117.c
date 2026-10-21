/*
 * A208117  Number of nX7 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence of order 16 for OEIS A208117 (number of
   n×7 0/1 arrays avoiding the patterns 000,010 horizontally and
   001,011 vertically).  The formula from the OEIS entry is:  
   a(n) = 2*a(n-1) + 6*a(n-2) - 14*a(n-3) - 14*a(n-4) + 42*a(n-5) 
          + 14*a(n-6) - 70*a(n-7) + 70*a(n-9) - 14*a(n-10) 
          - 42*a(n-11) + 14*a(n-12) + 14*a(n-13) - 6*a(n-14) 
          - 2*a(n-15) + a(n-16)
   with coefficients for a(n-8) equal to 0.  For n ≤ 16 the values
   are given explicitly. */

seq_t a(idx_t n)
{
    /* base terms a(1) … a(16) */
    static const seq_t base[16] = {
        40ULL,          /* a(1) */
        1600ULL,        /* a(2) */
        9240ULL,        /* a(3) */
        53361ULL,       /* a(4) */
        175560ULL,      /* a(5) */
        577600ULL,      /* a(6) */
        1432600ULL,     /* a(7) */
        3553225ULL,     /* a(8) */
        7419360ULL,     /* a(9) */
        15492096ULL,    /* a(10) */
        28791840ULL,    /* a(11) */
        53509225ULL,    /* a(12) */
        91408240ULL,    /* a(13) */
        156150016ULL,   /* a(14) */
        250232400ULL,   /* a(15) */
        401000625ULL    /* a(16) */
    };

    /* coefficients c[0] … c[15] correspond to a(n-1) … a(n-16) */
    static const int coeff[16] = {
        2,      /* a(n-1) */
        6,      /* a(n-2) */
        -14,    /* a(n-3) */
        -14,    /* a(n-4) */
        42,     /* a(n-5) */
        14,     /* a(n-6) */
        -70,    /* a(n-7) */
        0,      /* a(n-8) */
        70,     /* a(n-9) */
        -14,    /* a(n-10) */
        -42,    /* a(n-11) */
        14,     /* a(n-12) */
        14,     /* a(n-13) */
        -6,     /* a(n-14) */
        -2,     /* a(n-15) */
        1       /* a(n-16) */
    };

    if (n <= 16)
        return base[(unsigned)n - 1];

    /* sliding window of the last 16 terms */
    seq_t last[16];
    for (int i = 0; i < 16; ++i) last[i] = base[i];

    for (idx_t k = 17; k <= n; ++k) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        /* compute using the recurrence */
        for (int i = 0; i < 16; ++i) {
            int c = coeff[i];
            seq_t prod = (seq_t)(c >= 0 ? c : -c) * last[15 - i];
            if (c >= 0)
                sum_pos += prod;
            else
                sum_neg += prod;
        }
        seq_t new_val = sum_pos - sum_neg;

        /* shift window: drop oldest, append new_val */
        for (int i = 0; i < 15; ++i)
            last[i] = last[i + 1];
        last[15] = new_val;
    }

    return last[15];
}
