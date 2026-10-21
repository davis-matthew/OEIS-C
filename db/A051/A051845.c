/*
 * A051845  Triangle T(n,k) read by rows, in which row n gives all
 *          permutations of digits 1..n interpreted in base n+1.
 * https://oeis.org/A051845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 495, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A051845: read all permutations of digits 1..n in base n+1 row by row. */
/* The function returns the n‑th term of that reading. */

#include <stdbool.h>

static idx_t fact[21];
static bool fact_ready = false;

static void ensure_factorials(void)
{
    if (!fact_ready)
    {
        fact[0] = 1;
        for (int i = 1; i <= 20; ++i)
            fact[i] = fact[i-1] * (idx_t)i;
        fact_ready = true;
    }
}

seq_t a(idx_t n)
{
    /* prepare factorials once */
    ensure_factorials();

    /* locate the row r containing the term n */
    idx_t r   = 1;
    idx_t cum = 0;
    while (cum + fact[r] < n)
    {
        cum += fact[r];
        ++r;
    }

    /* zero‑based position within that row */
    idx_t pos = n - cum - 1;

    bool used[21] = {false};
    seq_t value = 0;

    /* produce the pos‑th lexicographic permutation of 1..r
       and compute its numeric value in base (r+1). */
    for (int i = r - 1; i >= 0; --i)
    {
        idx_t f   = fact[i];          /* (i)!  */
        idx_t idx = pos / f;          /* which unused digit to pick */
        pos %= f;

        int digit = 0;
        int count = 0;
        for (int d = 1; d <= r; ++d)
        {
            if (!used[d])
            {
                if (count == idx)
                {
                    digit   = d;
                    used[d] = true;
                    break;
                }
                ++count;
            }
        }

        value = value * (seq_t)(r + 1) + (seq_t)digit;
    }

    return value;
}
