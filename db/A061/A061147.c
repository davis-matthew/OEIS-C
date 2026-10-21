/*
 * A061147  Product of all distinct numbers formed by permuting digits of n.
 * https://oeis.org/A061147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 100, u32 n <= 101, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A061147 – Product of all distinct numbers formed by permuting
    the decimal digits of n (leading zeros are ignored).
*/
#include <stdint.h>
/* The types seq_t and idx_t are supplied by the build system. */

static void sort_digits(int *a, int n)
{
    /* simple insertion sort – digits are at most 20, so cost is negligible */
    for (int i = 1; i < n; ++i) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = key;
    }
}

static int next_permutation(int *a, int n)
{
    /* standard algorithm for the next lexicographic permutation.
       Returns 1 if a new permutation was generated, 0 if a was the last one. */
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1])
        --i;
    if (i < 0)
        return 0; /* last permutation reached */

    int j = n - 1;
    while (a[j] <= a[i])
        --j;

    /* swap a[i] and a[j] */
    int tmp = a[i]; a[i] = a[j]; a[j] = tmp;

    /* reverse suffix a[i+1 .. n-1] */
    int l = i + 1, r = n - 1;
    while (l < r) {
        tmp = a[l]; a[l] = a[r]; a[r] = tmp;
        ++l; --r;
    }
    return 1;
}

seq_t a(idx_t n)
{
    /* 1.  Extract decimal digits of n into an array (most significant first). */
    int digits[20];          /* enough for 64‑bit integers */
    int len = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        /* collect digits in reverse order */
        while (tmp > 0) {
            digits[len++] = tmp % 10;
            tmp /= 10;
        }
        /* reverse to get most significant first */
        for (int i = 0; i < len / 2; ++i) {
            int swap = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = swap;
        }
    }

    /* 2.  Sort the digits – needed for the next_permutation routine. */
    sort_digits(digits, len);

    /* 3.  Iterate over all distinct permutations, compute each number,
         multiply them together. */
    seq_t product = (seq_t)1;
    for (;;) {
        /* build the current number, skipping leading zeros. */
        seq_t value = (seq_t)0;
        int leading = 1;     /* true while leading zero(s) are being skipped */
        for (int i = 0; i < len; ++i) {
            if (leading && digits[i] == 0)
                continue;
            leading = 0;
            value = value * 10 + digits[i];
        }
        product *= value;

        if (!next_permutation(digits, len))
            break;
    }

    return product;
}
