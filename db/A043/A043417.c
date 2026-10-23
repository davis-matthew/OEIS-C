/*
 * A043417  Numbers whose base-7 representation contains exactly one 6.
 * https://oeis.org/A043417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number whose base‑7 representation contains exactly one 6
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find the length L of the base‑7 representation that contains
       the nth number.  Precompute powers of 6 as needed. */
    idx_t sum = 0;          /* cumulative counts of shorter lengths */
    idx_t L   = 1;          /* current length */
    seq_t pow6[128] = {0};  /* 6^i (i >= 0) */
    pow6[0] = 1;
    idx_t pow6_size = 1;    /* number of entries currently valid in pow6 */

    while (true) {
        idx_t countL;
        if (L == 1) {
            countL = 1;   /* only the number 6 */
        } else {
            /* count for length L: 6^(L-1) + 5*6^(L-2) = 11*6^(L-2) */
            /* ensure pow6[L-2] is computed */
            while (pow6_size <= L - 2) {
                pow6[pow6_size] = pow6[pow6_size - 1] * 6;
                pow6_size++;
            }
            countL = (idx_t)11 * (idx_t)pow6[L - 2];
        }
        if (sum + countL >= n)
            break;
        sum += countL;
        L++;
    }

    /* Position of the desired number within its length block */
    idx_t m = n - sum - 1;      /* 0‑based index */

    bool used6 = false;         /* whether the digit 6 has already been placed */
    unsigned digits[128];       /* digits from most significant to least */

    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t r = L - pos - 1;  /* remaining positions after current one */

        while (pow6_size <= r) { /* compute needed powers of 6 */
            pow6[pow6_size] = pow6[pow6_size - 1] * 6;
            pow6_size++;
        }

        unsigned d_start = (pos == 0) ? 1 : 0;
        for (unsigned d = d_start; d <= 6; ++d) {
            /* skip 6 if it has already been used */
            if (used6 && d == 6)
                continue;

            idx_t cnt;  /* number of completions after choosing digit d */

            if (used6) {                /* 6 already placed, no more 6 allowed */
                cnt = (idx_t)pow6[r];
            } else {                    /* 6 not yet used */
                if (d == 6) {           /* place the single 6 now */
                    cnt = (idx_t)pow6[r];
                } else {                 /* still need to place the 6 later */
                    if (r == 0)
                        cnt = 0;
                    else
                        cnt = r * (idx_t)pow6[r - 1];
                }
            }

            if (m >= cnt) {
                m -= cnt;
                continue;   /* try next digit */
            } else {
                digits[pos] = d;
                if (d == 6)
                    used6 = true;
                break;        /* go to next position */
            }
        }
    }

    /* Convert the digit array to a decimal value */
    seq_t value = 0;
    for (idx_t pos = 0; pos < L; ++pos) {
        value = value * 7 + digits[pos];
    }
    return value;
}
