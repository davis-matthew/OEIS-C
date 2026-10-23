/*
 * A118181  Row sums of triangle A118180: a(n) = Sum_{k=0..n} (3^k)^(n-k) for
 *          n>=0.
 * https://oeis.org/A118181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (3^k)^(n-k) = Sum_{k=0..n} 3^{k*(n-k)}
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Cache for powers of 3 up to the largest n requested so far */
static seq_t *pow3 = NULL;
static idx_t pow3_len = 0;

/* Ensure that pow3 contains 3^i for 0 <= i <= n */
static void ensure_pow3(idx_t n)
{
    /* Need array of size n+1 */
    idx_t new_len = n + 1;
    if (pow3_len >= new_len)
        return;

    seq_t *new_arr = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (pow3_len > 0)
        memcpy(new_arr, pow3, pow3_len * sizeof(seq_t));

    /* Compute new entries */
    for (idx_t i = pow3_len; i < new_len; ++i)
    {
        if (i == 0)
            new_arr[i] = 1;
        else
            new_arr[i] = new_arr[i - 1] * (seq_t)3;
    }

    free(pow3);
    pow3 = new_arr;
    pow3_len = new_len;
}

/* Compute the n-th term of sequence A118181 */
seq_t a(idx_t n)
{
    ensure_pow3(n);          /* guarantee 3^i up to n are ready */

    seq_t sum = 0;
    seq_t t = 1;             /* t_k = 3^{k*(n-k)} starts at k = 0 */

    for (idx_t k = 0; k <= n; ++k)
    {
        sum += t;
        if (k == n)
            break;

        /* delta = n - 2*k - 1 */
        int delta = (int)((int64_t)n - 2LL * (int64_t)k - 1LL);
        if (delta > 0)
            t *= pow3[delta];
        else if (delta < 0)
            t /= pow3[(idx_t)(-delta)];
        /* if delta == 0, t remains unchanged */
    }

    return sum;
}
