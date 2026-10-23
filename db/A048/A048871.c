/*
 * A048871  Length of hypotenuse squared in right triangle formed by a
 *          palindromic spiral plotted in Cartesian coordinates.
 * https://oeis.org/A048871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, nice
 * Formula:   a(n) = A002113(n)^2 + A002113(n-1)^2, n > 1. A002113 is the
 *            palindromic‑number sequence in base‑10, with a duplicate leading
 *            1 at indices 0 and 1
 * Range:     u8 n <= 10, u16 n <= 27, u32 n <= 562, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow10, reverse_first_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/reverse_first_digits.h"

static inline idx_t pow10_idx(idx_t p)
{
    idx_t r = 1;
    for (idx_t i = 0; i < p; ++i)
        r *= 10;
    return r;
}

/* A002113(k): 0‑based index.  */
static seq_t A002113(idx_t k)
{
    if (k == 0 || k == 1) return 1;

    /* Position inside the ordinary palindrome list. */
    idx_t pos = k - 1;

    idx_t cum   = 0;      /* total palindromes processed so far   */
    idx_t l     = 1;      /* current length of palindromes         */
    idx_t count = 9;      /* how many palindromes of length l      */

    /* Find the block that contains the desired palindrome. */
    while (cum + count <= pos) {
        cum += count;
        ++l;

        idx_t half = l / 2;
        if (l % 2 == 0) {                 /* even length 2*half */
            count = (idx_t)9 * pow10_idx(half - 1);
        } else {                          /* odd length 2*half+1 */
            count = (idx_t)9 * pow10_idx(half);
        }
    }

    idx_t offset = pos - cum;              /* 0‑based inside the block   */
    idx_t half   = l / 2;                  /* number of free digits before the middle    */
    seq_t pow10k = pow10((idx_t)half);     /* 10^half as seq_t                               */

    seq_t prefix_start;
    if (l % 2 == 0)                        /* even length */
        prefix_start = pow10((idx_t)(half - 1));
    else                                    /* odd length  */
        prefix_start = pow10((idx_t)half);

    seq_t prefix = prefix_start + (seq_t)offset;

    seq_t palindrome;
    if (l % 2 == 0) {                      /* even length: mirror the whole prefix  */
        palindrome = prefix * pow10k + reverse_first_digits(prefix, half);
    } else {                                /* odd length: drop middle digit  */
        seq_t without_middle = prefix / 10;
        palindrome = prefix * pow10k + reverse_first_digits(without_middle, half);
    }
    return palindrome;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* below the offset – undefined value */

    seq_t bn   = A002113(n);
    seq_t bn_1 = A002113(n - 1);
    return bn * bn + bn_1 * bn_1;
}
