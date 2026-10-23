/*
 * A049921  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m =
 *          2^(p+1) + 2 - n and p is the unique integer such that 2^p < n - 1
 *          <= 2^(p+1), starting with a(1) = 1, a(2) = 3, and a(3) = 2.
 * https://oeis.org/A049921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049921
 *
 * a(1) = 1 ,  a(2) = 3 ,  a(3) = 2
 * For n ≥ 4:
 *    p is the unique integer with 2^p < n-1 ≤ 2^(p+1)
 *    m = 2^(p+1) + 2 – n
 *    a(n) = (a(1)+a(2)+···+a(n-1)) – a(m)
 *
 * The implementation stores both the terms and their prefix sums
 * in static arrays, expanding the allocation as needed.  It works
 * for any unsigned integer types supplied as seq_t (value) and
 * idx_t (index) and obeys all listed constraints. */

#include <stdlib.h>

#define LOG2_FLOOR(v)                                            \
    ({ idx_t _v = (v); idx_t _p = 0;                            \
       while (((idx_t)1 << (_p + 1)) <= _v) ++_p;                \
       _p; })

static seq_t *seq    = NULL;   /* cache of a(i) values   */
static seq_t *prefix = NULL;   /* cache of prefix sums   */
static idx_t  size   = 0;      /* current allocation size */
static idx_t  last   = 0;      /* largest n computed so far */

static void
ensure_capacity(idx_t n)
{
    if (n <= size)
        return;
    idx_t new_size = size ? size * 2 : 4;
    while (new_size < n)
        new_size <<= 1;
    seq    = (seq_t *)realloc(seq,    new_size * sizeof(seq_t));
    prefix = (seq_t *)realloc(prefix, new_size * sizeof(seq_t));
    size   = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined but safe */

    ensure_capacity(n);

    for (idx_t i = last + 1; i <= n; ++i) {
        idx_t idx = i - 1;  /* array index for term i */

        if (i == 1) {
            seq[idx] = (seq_t)1;
        } else if (i == 2) {
            seq[idx] = (seq_t)3;
        } else if (i == 3) {
            seq[idx] = (seq_t)2;
        } else {
            seq_t sum = prefix[idx - 1];                /* Σ_{k=1}^{i-1} a(k) */

            idx_t nm1 = i - 1;                          /* n-1  */
            idx_t p   = LOG2_FLOOR(nm1 - 1);            /* floor(log2(n-1-1)) */
            idx_t two_pow = ((idx_t)1 << (p + 1));      /* 2^(p+1)          */
            idx_t m     = two_pow + 2 - i;              /* m as defined above */

            seq[idx] = sum - seq[m - 1];
        }

        /* update prefix sum */
        if (i == 1) {
            prefix[idx] = seq[idx];
        } else {
            prefix[idx] = prefix[idx - 1] + seq[idx];
        }
    }

    last = n;
    return seq[n - 1];
}
