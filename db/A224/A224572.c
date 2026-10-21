/*
 * A224572  Number of (n+5) X 8 0..1 matrices with each 6 X 6 subblock
 *          idempotent.
 * https://oeis.org/A224572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+5)×8 0..1 matrices with each 6×6 subblock
 *            idempotent Recurrence (empirical, valid for n>21): a(n) =
 *            3*a(n-1) -4*a(n-2) +4*a(n-3) -3*a(n-4) + a(n-5) +2*a(n-6)
 *            -5*a(n-7) +6*a(n-8) -6*a(n-9) +4*a(n-10) -a(n-11) -a(n-12)
 *            +2*a(n-13) -2*a(n-14) +2*a(n-15) -a(n-16) Initial values (n...
 * Range:     u8 none, u16 n <= 16, u32 n <= 60, u64 n <= 149, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[21] = {
    16696ul, 5696ul, 5896ul, 6659ul, 7295ul,
    7884ul, 8294ul, 9831ul, 12556ul, 16161ul,
    20310ul, 25054ul, 30119ul, 36575ul, 45620ul,
    58199ul, 74850ul, 96163ul, 122349ul, 154920ul,
    196491ul
};

static const int coeffs[16] = {3,-4,4,-3,1,2,-5,6,-6,4,-1,-1,2,-2,2,-1};

seq_t a(idx_t n)
/* Computes the nth term of OEIS A224572. */
{
    idx_t idx = n - 1;               /* zero‑based index */
    size_t len = (size_t)idx + 1;    /* number of terms needed */

    seq_t *arr = (seq_t *)malloc(len * sizeof(seq_t));
    if (!arr) return 0;              /* allocation failure – unlikely */

    /* initialise known initial values */
    for (int i = 0; i < 21 && i < (int)len; ++i)
        arr[i] = base_vals[i];

    /* compute remaining terms using the recurrence */
    for (size_t i = 21; i < len; ++i) {
        seq_t pos = 0, neg = 0;
        for (int j = 0; j < 16; ++j) {
            seq_t term = arr[i - 1 - j];
            int c = coeffs[j];
            if (c > 0)
                pos += (seq_t)c * term;
            else
                neg += (seq_t)(-c) * term;   /* add the positive counterpart */
        }
        arr[i] = pos - neg;    /* safe: pos >= neg for all n */
    }

    seq_t result = arr[idx];
    free(arr);

    return result;
}
