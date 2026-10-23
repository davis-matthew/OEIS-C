/*
 * A043486  Numbers having two 8's in base 9.
 * https://oeis.org/A043486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043486: numbers having two 8's in base 9. */

#include <stddef.h>

/* Compute 8^exp as a seq_t value.  exp is non‑negative. */
static seq_t pow8(idx_t exp)
{
    seq_t r = (seq_t)1;
    for (idx_t i = (idx_t)0; i < exp; ++i)
        r = r * (seq_t)8;
    return r;
}

/* Compute binomial coefficient C(n,k) as seq_t.  n,k are non‑negative.
   Uses an iterative multiplicative algorithm to stay within bounds. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;
    seq_t r = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
    {
        r = r * (seq_t)(n - k + i) / (seq_t)i;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;                 /* undefined for n < 1 */

    /* Find the number of base‑9 digits (L) of the answer. */
    idx_t L = (idx_t)2;
    seq_t cumulative = (seq_t)0;

    while (1)
    {
        /* Count of length‑L numbers with exactly two 8's. */
        seq_t count_lead8 = (seq_t)(L - (idx_t)1) * pow8(L - (idx_t)2);
        seq_t count_nolead = (seq_t)0;
        if (L >= (idx_t)3)
            count_nolead = (seq_t)7 * binom(L - (idx_t)1, (idx_t)2) * pow8(L - (idx_t)3);
        seq_t total = count_lead8 + count_nolead;

        if (cumulative + total >= (seq_t)n)
            break;
        cumulative += total;
        ++L;
    }

    seq_t offset = (seq_t)n - cumulative;

    unsigned char digits[64];
    seq_t count8 = (seq_t)0;

    for (idx_t pos = (idx_t)0; pos < L; ++pos)
    {
        unsigned int d_min = (pos == (idx_t)0) ? 1 : 0;
        for (unsigned int d = d_min; d <= 8; ++d)
        {
            seq_t new_count8 = count8 + (d == 8 ? (seq_t)1 : (seq_t)0);
            idx_t new8 = (idx_t)new_count8;
            seq_t ways = (seq_t)0;

            if (new8 <= (idx_t)2)
            {
                idx_t rem = L - pos - (idx_t)1;
                idx_t needed = (idx_t)2 - new8;
                if (needed <= rem)
                    ways = binom(rem, needed) * pow8(rem - needed);
            }

            if (offset <= ways)
            {
                digits[pos] = (unsigned char)d;
                count8 = new_count8;
                break;
            }
            else
            {
                offset -= ways;
            }
        }
    }

    /* Convert the base‑9 digits to a decimal (seq_t) value. */
    seq_t result = (seq_t)0;
    for (idx_t i = (idx_t)0; i < L; ++i)
        result = result * (seq_t)9 + (seq_t)digits[i];

    return result;
}
