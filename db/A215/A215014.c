/*
 * A215014  Numbers where any two consecutive decimal digits differ by 1 after
 *          arranging the digits in decreasing order.
 * https://oeis.org/A215014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2245, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Enumerates all numbers whose digits are a permutation of a contiguous block of distinct digits
   (maximum digit – minimum digit + 1 == number of digits).  It generates the full sequence up to the
   4 091 131st term, sorts it ascending, and returns the nth element. */
#include <stdlib.h>
#include <stdint.h>

static size_t factorial(int n)
{
    size_t f = 1;
    for (int i = 2; i <= n; ++i) f *= i;
    return f;
}

static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

static void permute(seq_t *seq, idx_t *len, unsigned char *digits, int k, int pos)
{
    if (pos == k) {
        if (digits[0] == 0) return;          /* leading zero is invalid */
        uint64_t val = 0;
        for (int i = 0; i < k; ++i) val = val * 10 + digits[i];
        seq[(*len)++] = (seq_t)val;
        return;
    }
    for (int i = pos; i < k; ++i) {
        unsigned char tmp = digits[pos];
        digits[pos] = digits[i];
        digits[i] = tmp;
        permute(seq, len, digits, k, pos + 1);
        /* backtrack */
        tmp = digits[pos];
        digits[pos] = digits[i];
        digits[i] = tmp;
    }
}

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static idx_t seq_len = 0;
    static size_t seq_cap = 0;

    if (seq == NULL) {
        /* Compute total number of terms: 1 (for zero) + sum over k=1..10 of
           (11-k)*k! - (k-1)! */
        seq_cap = 1;
        for (int k = 1; k <= 10; ++k) {
            size_t term = (size_t)(11 - k) * factorial(k);
            term -= factorial(k - 1);
            seq_cap += term;
        }
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;

        seq[seq_len++] = (seq_t)0;    /* a(1) = 0 */

        unsigned char digits[10];
        for (int k = 1; k <= 10; ++k) {
            for (int start = 0; start <= 9 - (k - 1); ++start) {
                for (int i = 0; i < k; ++i) digits[i] = start + i;
                permute(seq, &seq_len, digits, k, 0);
            }
        }
        qsort(seq, seq_len, sizeof(seq_t), cmp_seq_t);
    }
    return seq[(size_t)(n - 1)];
}
