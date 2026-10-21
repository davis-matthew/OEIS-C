/*
 * A050049  a(n) = a(n-1) + a(m) for n >= 3, where m = 2^(p+1) + 2 - n and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = 1 and a(2) = 2.
 * https://oeis.org/A050049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(m) for n>=3, where m = 2^(p+1) + 2 - n and p =
 *            floor(log2(n-2))
 * Range:     u8 n <= 19, u16 n <= 72, u32 n <= 515, u64 n <= 4335,
 *            u128 n <= 8193, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq = NULL;          /* array of sequence values */
static size_t seq_len = 0;         /* allocated length of seq[]  */
static idx_t  max_computed = 0;    /* largest n that has been filled */

/* Ensure the array has room for indices up to n (inclusive) */
static void ensure_size(idx_t n)
{
    if (seq_len <= n) {
        size_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_len * sizeof(seq_t));
        if (!tmp) abort();
        seq   = tmp;
        seq_len = new_len;
    }
}

/* Return the n-th term of A050049, 1‑based */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    ensure_size(n);

    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 1) { seq[1] = 1; continue; }
            if (i == 2) { seq[2] = 2; continue; }

            idx_t t = i - 2;          /* t = n-2 */
            idx_t p = 0;
            while (t >> 1) { t >>= 1; ++p; }   /* p = floor(log2(n-2)) */

            idx_t pow2 = 1ULL << (p + 1);       /* 2^(p+1) */
            idx_t m    = pow2 + 2 - i;          /* m = 2^(p+1)+2-n */

            seq[i] = seq[i - 1] + seq[m];
        }
        max_computed = n;
    }

    return seq[n];
}
