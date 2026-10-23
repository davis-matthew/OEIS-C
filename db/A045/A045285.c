/*
 * A045285  Numbers whose base-5 representation contains exactly two 2's and
 *          no 4's.
 * https://oeis.org/A045285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer whose base‑5 representation contains exactly
 *            two 2’s and no 4’s. The algorithm finds the required number of
 *            digits, then un‑ranks the residual index using combinatorial
 *            counting
 * Range:     u8 n <= 19, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* precomputation limits – enough for n up to about 10000 */
#define MAX_DIGITS 12

static seq_t pow3[MAX_DIGITS];
static seq_t choose[MAX_DIGITS][MAX_DIGITS];
static bool tables_ready = false;

/* pre‑compute 3^i and binomial coefficients */
static void init_tables(void)
{
    if (tables_ready) return;
    pow3[0] = (seq_t)1;
    for (int i = 1; i < MAX_DIGITS; ++i)
        pow3[i] = pow3[i - 1] * (seq_t)3;

    for (int i = 0; i < MAX_DIGITS; ++i) {
        for (int j = 0; j <= i; ++j) {
            if (j == 0 || j == i)
                choose[i][j] = (seq_t)1;
            else
                choose[i][j] = choose[i - 1][j - 1] + choose[i - 1][j];
        }
    }
    tables_ready = true;
}

/* count of admissible numbers of exactly 'len' base‑5 digits (len >= 2) */
static seq_t count_len(int len)
{
    if (len == 2) return (seq_t)1;
    /* (len−1)*(len+1)*3^(len−3) */
    return (seq_t)(len - 1) * (seq_t)(len + 1) * pow3[len - 3];
}

seq_t a(idx_t n)
{
    init_tables();

    /* 1. find the total digit length of the answer */
    int len = 2;
    seq_t remaining = (seq_t)n;          /* 1‑based index within the whole sequence
                                           (converted to seq_t for uniformity) */
    while (1) {
        seq_t cnt = count_len(len);
        if (remaining <= cnt) break;
        remaining -= cnt;
        ++len;
    }

    /* 2. un‑rank the residual index within numbers of this length */
    seq_t result = (seq_t)0;
    int rem_two = 2;                    /* two '2's still to place */

    for (int pos = 0; pos < len; ++pos) {
        /* possible digits for this position, in ascending order */
        int cand[4], ncand = 0;
        if (pos == 0) {
            cand[ncand++] = 1;
            cand[ncand++] = 2;
            cand[ncand++] = 3;
        } else {
            cand[ncand++] = 0;
            cand[ncand++] = 1;
            cand[ncand++] = 2;
            cand[ncand++] = 3;
        }

        int rem_pos = len - pos - 1;          /* positions remaining after this one */

        for (int i = 0; i < ncand; ++i) {
            int d = cand[i];
            int rem_two_next = rem_two - (d == 2);

            if (rem_two_next < 0 || rem_two_next > rem_pos)
                continue;                     /* impossible choice */

            seq_t cnt = choose[rem_pos][rem_two_next] *
                        pow3[rem_pos - rem_two_next];

            if (remaining <= cnt) {
                /* choose this digit */
                result = result * (seq_t)5 + (seq_t)d;
                rem_two = rem_two_next;
                break;
            } else {
                remaining -= cnt;
            }
        }
    }

    return result;
}
