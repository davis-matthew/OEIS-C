/*
 * A049929  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m =
 *          2^(p+1) + 2 - n and p is the unique integer such that 2^p < n-1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 3, and a(3) = 4.
 * https://oeis.org/A049929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=4; for n≥4
 *            a(n)=∑_{i=1}^{n-1}a(i)-a(2^⌈log₂(n-1)⌉+2-n)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *val = NULL;       /* values a(1),a(2),… */
static seq_t *pre = NULL;       /* prefix sums pre[i]=∑_{j=1}^{i}a(j) */
static size_t cap = 0;          /* allocated capacity (indices 0…cap-1) */
static idx_t  computed = 0;     /* highest index computed so far */

/* ensure arrays are large enough for indices up to n */
static void ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) <= cap)
        return;

    size_t newcap = cap ? cap << 1 : 8;
    while (newcap <= (size_t)(n + 1))
        newcap <<= 1;

    /* reallocate val */
    val = (seq_t*)realloc(val, newcap * sizeof(seq_t));
    pre = (seq_t*)realloc(pre, newcap * sizeof(seq_t));

    /* zero new entries to avoid garbage during subtraction */
    for (size_t i = cap; i < newcap; ++i) {
        val[i] = 0;
        pre[i] = 0;
    }
    cap = newcap;
}

/* extend table up to index n */
static void compute_up_to(idx_t n)
{
    if (n <= computed)
        return;

    ensure_capacity(n);

    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1) {
            val[1] = 1;
            pre[1] = 1;
        } else if (i == 2) {
            val[2] = 3;
            pre[2] = pre[1] + val[2]; /* 1 + 3 = 4 */
        } else if (i == 3) {
            val[3] = 4;
            pre[3] = pre[2] + val[3]; /* 4 + 4 = 8 */
        } else {
            /* compute p such that 2^p < i-1 <= 2^(p+1)   */
            idx_t pow = 1;
            while (pow < (i - 1))
                pow <<= 1;                /* now pow == 2^⌈log₂(i-1)⌉ */

            idx_t m = pow + 2 - i;            /* index to exclude */
            val[i] = pre[i - 1] - val[m];     /* a(i) = sum_{1}^{i-1} - a(m) */
            pre[i] = pre[i - 1] + val[i];     /* update running sum */
        }
    }
    computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1)  /* undefined for n=0 or negative; return 0 */
        return (seq_t)0;

    compute_up_to(n);
    return val[n];
}
