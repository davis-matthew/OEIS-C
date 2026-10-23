/*
 * A033004  Every run of digits of n in base 6 has length 2.
 * https://oeis.org/A033004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 7 * X, where X is the *n*-th number whose * base‑36
 *            representation consists of digits 0–5 (most significant * digit
 *            1–5) and has no two equal adjacent digits. * Another equivalent
 *            description: a(n) are numbers whose base‑6 * representation has
 *            blocks of equal digits, each block of length *...
 * Range:     u8 n <= 6, u16 n <= 155, u32 n <= 700, u64 n <= 700,
 *            u128 n <= 700, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 0 is undefined – return 0 for safety */
    if (n == 0)
        return 0;

    /* Find the block length k (number of digit blocks in the
     * base‑6 representation, which equals the number of base‑36 digits) */
    idx_t cum = 0;          /* count of numbers in previous blocks   */
    idx_t pow = 5;          /* 5^1                                     */
    unsigned k = 1;         /* current block length                   */

    /* Determine k such that n lies in the block of length k */
    while (n > cum + pow) {
        cum += pow;                                 /* all numbers in earlier blocks          */
        if (pow > ((idx_t)-1) / 5)                  /* would overflow – stop (not expected)    */
            break;
        pow *= 5;                                   /* pow = 5^k                              */
        k++;
    }

    /* Offset of n within its block (0‑based) */
    idx_t start_idx = cum + 1;
    idx_t t = n - start_idx;

    /* Precompute powers of 5 up to k */
    idx_t pow5[64];
    pow5[0] = 1;
    for (unsigned i = 1; i <= k; ++i)
        pow5[i] = pow5[i - 1] * 5;

    /* First digit (most significant in base‑36 / base‑6) */
    seq_t first = (seq_t)(t / pow5[k - 1]) + 1;
    t %= pow5[k - 1];

    /* Build the base‑36 number X digit by digit */
    seq_t X = first;
    seq_t prev = first;

    for (unsigned pos = 2; pos <= k; ++pos) {
        unsigned rem_len = k - pos;                     /* remaining positions after this one */
        idx_t group_size = pow5[rem_len];                /* completions for the suffix      */
        idx_t idx = t / group_size;                     /* which option of the allowed set*/
        t %= group_size;

        seq_t d = (seq_t)idx;                           /* candidate digit (0–4) */
        if (d >= prev) d++;                              /* skip the previous digit   */

        X = X * (seq_t)36 + d;
        prev = d;
    }

    /* a(n) = 7 * X */
    return X * (seq_t)7;
}
