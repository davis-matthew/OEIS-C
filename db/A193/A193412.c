/*
 * A193412  1..9 followed by the canyon numbers A134970.
 * https://oeis.org/A193412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   all integers whose decimal digits form a single valley (strictly
 *            decreasing to a unique minimum, then strictly increasing), with
 *            the first and last digit equal and in 1..9. The sequence is
 *            finite, ends with 9876543210123456789
 * Range:     u8 none, u16 none, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Comparator for qsort. */
static int cmp_seq(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    return (*a < *b) ? -1 : (*a > *b) ? 1 : 0;
}

/* Cache of all sequence terms. */
static seq_t *seq_cache = NULL;
static idx_t seq_len   = 0;

/* Build the full sequence once. */
static void build_sequence(void)
{
    /* Count how many numbers there will be. */
    size_t total = 9;          /* the single digits 1..9 */
    for (int d = 1; d <= 9; ++d)
    {
        for (int m = 0; m < d; ++m)
        {
            size_t avail = (size_t)(d - 1 - m);   /* digits > m and < d */
            /* Each side can have any subset of the available digits. */
            size_t leftmax  = (size_t)1 << avail;
            size_t rightmax = leftmax;
            total += leftmax * rightmax;          /* 2^avail * 2^avail = 4^avail */
        }
    }

    seq_cache = (seq_t *)malloc(total * sizeof(seq_t));
    if (seq_cache == NULL) return; /* allocation failure -> empty sequence */

    size_t idx = 0;

    /* Single-digit terms 1..9. */
    for (int d = 1; d <= 9; ++d)
        seq_cache[idx++] = (seq_t)d;

    /* Valley terms with equal first and last digit. */
    for (int d = 1; d <= 9; ++d)
    {
        for (int m = 0; m < d; ++m)
        {
            size_t avail = (size_t)(d - 1 - m);   /* digits in (m, d) */
            size_t leftmax  = (size_t)1 << avail;
            size_t rightmax = leftmax;
            size_t base_digit = (size_t)(m + 1);

            for (size_t leftMask = 0; leftMask < leftmax; ++leftMask)
            {
                for (size_t rightMask = 0; rightMask < rightmax; ++rightMask)
                {
                    seq_t val = (seq_t)d;

                    /* Left side: digits in decreasing order. */
                    for (int i = (int)avail - 1; i >= 0; --i)
                    {
                        if (leftMask & ((size_t)1 << i))
                        {
                            val = val * ((seq_t)10) + (seq_t)(base_digit + i);
                        }
                    }

                    /* Minimal digit. */
                    val = val * ((seq_t)10) + (seq_t)m;

                    /* Right side: digits in increasing order. */
                    for (size_t i = 0; i < avail; ++i)
                    {
                        if (rightMask & ((size_t)1 << i))
                        {
                            val = val * ((seq_t)10) + (seq_t)(base_digit + i);
                        }
                    }

                    /* Trailing digit (equal to first). */
                    val = val * ((seq_t)10) + (seq_t)d;

                    seq_cache[idx++] = val;
                }
            }
        }
    }

    seq_len = (idx_t)idx;
    qsort(seq_cache, seq_len, sizeof(seq_t), cmp_seq);
}

/* Public function required by the assessment. */
seq_t a(idx_t n)
{
    static bool built = false;
    if (!built)
    {
        build_sequence();
        built = true;
    }

    if (n < (idx_t)1 || n > seq_len) return (seq_t)0;
    return seq_cache[n - 1];
}
