/*
 * A049951  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = 2*n
 *          - 2 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), with a(1) = 1, a(2) = 2, and a(3) = 1.
 * https://oeis.org/A049951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)+…+a(n-1) + a(m) for n ≥ 4 where m = 2*n – 2 –
 *            2^(p+1) and p = ⌊log₂(n−2)⌋. Initial values: a(1)=1, a(2)=2,
 *            a(3)=1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>   /* for uint64_t */

static seq_t *val = NULL;          /* a(n) values, 1‑based index */
static seq_t *pre = NULL;          /* prefix sums: pre[k] = a(1)+…+a(k) */
static size_t cap = 0;             /* capacity of allocated arrays */
static idx_t last = 0;             /* largest n already computed */

/* ensure that arrays have room for indices up to n (≥1) */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < cap)
        return;

    size_t new_cap = (cap == 0) ? 8 : cap;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *new_val;
    seq_t *new_pre;

    new_val = (seq_t *)realloc(val, new_cap * sizeof(seq_t));
    new_pre = (seq_t *)realloc(pre, new_cap * sizeof(seq_t));
    if (!new_val || !new_pre)
        abort();              /* allocation failed */

    /* initialise new slots */
    for (size_t i = cap; i < new_cap; ++i) {
        new_val[i] = 0;
        new_pre[i] = 0;
    }

    val = new_val;
    pre = new_pre;
    cap = new_cap;
    pre[0] = 0;               /* prefix sum of zero terms is zero */
}

/* compute the n‑th term of A049951, memoised and independent of call order */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* unused: safety */

    ensure_capacity(n);

    if (last < n) {
        for (idx_t k = last + 1; k <= n; ++k) {
            seq_t ak;
            if (k == 1)
                ak = 1;
            else if (k == 2)
                ak = 2;
            else if (k == 3)
                ak = 1;
            else {
                /* p = floor(log2(k-2)) : largest p with 2^p < k-1 <= 2^(p+1) */
                unsigned int p = 63 - __builtin_clzll((uint64_t)(k - 2));
                uint64_t pow2 = ((uint64_t)1) << (p + 1);
                idx_t m = 2 * k - 2 - (idx_t)pow2;   /* 1 ≤ m < k */
                ak = pre[k - 1] + val[m];
            }
            val[k] = ak;
            pre[k] = pre[k - 1] + ak;
        }
        last = n;
    }
    return val[n];
}
