/*
 * A043515  Numbers having three 6's in base 10.
 * https://oeis.org/A043515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 356, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers having exactly three 6's in decimal representation */
/* a(1) = 666, a(2) = 1666, a(3) = 2666, ... */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

/* small binomial coefficient, k <= 3 (used for all needed values) */
static seq_t comb_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i; /* division exact for binomial */
    return res;
}

/* compute the nth term of sequence A043515 */
seq_t a(idx_t n)
{
    /* 1‑based indexing; n >= 1 */
    seq_t n_seq = (seq_t)n;

    /* find the length L such that the nth term lies in numbers of this length */
    idx_t    L = 3;
    seq_t    cumulative = 0;
    for (;; ++L)
    {
        seq_t count_len;
        if (L == 3)
            count_len = 1;                                   /* only 666 */
        else
        {
            /* leading digit is 6 */
            seq_t a_part = comb_small(L - 1, 2) *
                           pow_seq(9, L - 3);                    /* C(L-1,2)*9^(L-3) */

            /* leading digit not 6 */
            seq_t b_part = 8 * comb_small(L - 1, 3) *
                           pow_seq(9, L - 4);                   /* 8*C(L-1,3)*9^(L-4) */

            count_len = a_part + b_part;
        }
        if (cumulative + count_len >= n_seq)
            break;
        cumulative += count_len;
    }

    /* index inside the block of numbers of length L (1‑based) */
    seq_t kth = n_seq - cumulative;

    seq_t value = 0;          /* resulting number */
    idx_t count_six = 0;     /* number of 6's already placed */

    for (idx_t pos = 1; pos <= L; ++pos)
    {
        for (int d = 0; d <= 9; ++d)
        {
            if (pos == 1 && d == 0)          /* no leading zero */
                continue;

            idx_t new_six = count_six + (d == 6 ? 1 : 0);
            idx_t rem_pos  = L - pos;
            idx_t rem_six  = 3 - new_six;

            seq_t cnt = 0;
            if (rem_six <= rem_pos && rem_six <= 3 && rem_six >= 0)
            {
                seq_t comb = comb_small(rem_pos, rem_six);
                seq_t pow9 = pow_seq(9, rem_pos - rem_six);
                cnt = comb * pow9;
            }

            if (cnt < kth)
            {
                kth -= cnt;
                continue;
            }

            /* choose this digit */
            value = value * 10 + d;
            count_six = new_six;
            break;
        }
    }

    return value;
}
