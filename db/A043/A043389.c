/*
 * A043389  Numbers having one 5 in base 6.
 * https://oeis.org/A043389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having exactly one digit 5 in base 6 (OEIS A043389) */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special handling for the first term */
    if (n == 1) {
        return (seq_t)5;
    }

    seq_t nseq = (seq_t)n;       /* working copy in seq_t */
    seq_t sum   = 0;             /* cumulative count of numbers processed */
    seq_t pow5_ = 1;             /* 5^(l-2) for current length l (valid for l>=2) */
    idx_t L = 0;                 /* length (number of base‑6 digits) of the desired term */

    /* Find the length L such that a(n) has L base‑6 digits */
    for (idx_t l = 1; ; ++l) {
        seq_t N;
        if (l == 1) {
            N = 1;   /* only the number 5 */
        } else {
            N = pow5_ * ((seq_t)(4*l + 1));  /* N(l) = 5^(l-2) * (4l + 1) */
            pow5_ *= 5;                      /* prepare pow5_ for next l      */
        }
        if (sum + N >= nseq) {
            L = l;
            break;
        }
        sum += N;
    }

    /* Index within the numbers that have exactly L digits (0‑based) */
    seq_t idx = nseq - sum - (seq_t)1;

    /* Precompute powers of 5 up to L (needed for counting completions) */
    seq_t pow5[64];
    pow5[0] = (seq_t)1;
    for (idx_t i = 1; i <= L; ++i) {
        pow5[i] = pow5[i-1] * (seq_t)5;
    }

    /* Generate the L base‑6 digits in lexicographic order */
    uint8_t digits[64];
    bool used5 = false;

    for (idx_t i = 0; i < L; ++i) {
        uint8_t start = (i == 0) ? 1 : 0;    /* most significant digit cannot be 0 */
        uint8_t end   = 5;
        idx_t rem = L - i - 1;               /* remaining positions after this one */

        for (uint8_t d = start; d <= end; ++d) {
            bool newUsed5 = used5 || (d == 5);
            seq_t count;

            if (newUsed5) {
                if (rem == 0) {
                    count = (seq_t)1;
                } else {
                    count = pow5[rem];
                }
            } else {   /* 5 not yet used */
                if (rem == 0) {
                    count = (seq_t)0;
                } else {
                    count = (seq_t)rem * pow5[rem - 1];
                }
            }

            if (idx < count) {
                digits[i] = d;
                used5 = newUsed5;
                break;
            } else {
                idx -= count;
            }
        }
    }

    /* Convert the digit array to a seq_t value */
    seq_t value = 0;
    for (idx_t i = 0; i < L; ++i) {
        value = value * (seq_t)6 + (seq_t)digits[i];
    }

    return value;
}
