/*
 * A226819  Numbers of the form 6^j + 7^k, for j and k >= 0.
 * https://oeis.org/A226819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest integer of the form 6^j + 7^k (j,k ≥ 0)
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 156, u64 n <= 575,
 *            u128 n <= 2300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Comparator for qsort; sorts seq_t values in ascending order. */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined offset, return 0 */

    static bool built    = false;
    static seq_t *uniq   = NULL;      /* sorted unique values */
    static size_t uniqc  = 0;         /* amount stored */

    if (!built)
    {
        /* --- 1. Generate all powers of 6 and 7 up to a modest exponent --- */
        const size_t MAX_EXP = 200;   /* large enough for n≈10 000, even 128‑bit */
        size_t sz6 = 0, sz7 = 0;
        seq_t *pow6 = (seq_t *)malloc(MAX_EXP * sizeof(seq_t));
        seq_t *pow7 = (seq_t *)malloc(MAX_EXP * sizeof(seq_t));
        if (!pow6 || !pow7) { free(pow6); free(pow7); return (seq_t)0; }

        pow6[sz6++] = (seq_t)1;         /* 6^0 */
        for (size_t i = 1; i < MAX_EXP; ++i)
        {
            seq_t next = pow6[i-1] * (seq_t)6;
            if (next < pow6[i-1]) break; /* overflow in unsigned types */
            pow6[sz6++] = next;
        }
        pow7[sz7++] = (seq_t)1;         /* 7^0 */
        for (size_t i = 1; i < MAX_EXP; ++i)
        {
            seq_t next = pow7[i-1] * (seq_t)7;
            if (next < pow7[i-1]) break;
            pow7[sz7++] = next;
        }

        /* --- 2. Compute all possible sums 6^j+7^k --- */
        size_t total = sz6 * sz7;
        seq_t *tmp = (seq_t *)malloc(total * sizeof(seq_t));
        if (!tmp) { free(pow6); free(pow7); return (seq_t)0; }
        size_t cnt = 0;
        for (size_t i = 0; i < sz6; ++i)
        {
            for (size_t j = 0; j < sz7; ++j)
            {
                /* no overflow guard for big‑int; but for unsigned types the
                   check below ensures no wrap unless overflow already occurs */
                seq_t sum = pow6[i] + pow7[j];
                /* For unsigned types, overflow would wrap around, but we
                   only accept the sum if it is strictly larger than both addends.
                   For big‑int, this test never fails. */
                if (sum >= pow6[i] && sum >= pow7[j])
                    tmp[cnt++] = sum;
            }
        }
        free(pow6);
        free(pow7);

        /* --- 3. Sort and deduplicate --- */
        qsort(tmp, cnt, sizeof(seq_t), cmp_seq_t);

        uniqc = 0;
        if (cnt > 0)
        {
            uniq = (seq_t *)malloc(cnt * sizeof(seq_t));
            if (!uniq) { free(tmp); return (seq_t)0; }
            uniq[0] = tmp[0];
            uniqc = 1;
            for (size_t i = 1; i < cnt; ++i)
                if (tmp[i] != uniq[uniqc-1])
                    uniq[uniqc++] = tmp[i];
        }
        free(tmp);
        built = true;
    }

    if ((idx_t)uniqc < n) return (seq_t)0;      /* n beyond precomputed range */
    return uniq[n-1];
}
