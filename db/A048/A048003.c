/*
 * A048003  Triangular array T read by rows: T(h,k) = number of binary words
 *          of length h and maximal runlength k.
 * https://oeis.org/A048003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = ceil((sqrt(8*n+1)-1)/2) and c = n -
 *            r*(r-1)/2. T(n,k) counts binary words of length n with maximal
 *            runlength k: T(n,1) = T(n,n) = 2 for 1<k<n: T(n,k) = 2*T(n-1,k)
 *            + T(n-1,k-1) - 2*T(n-2,k-1) + T(n-k,k-1) - T(n-k-1,k) (all
 *            outside‑range values are 0)
 * Range:     u8 n <= 47, u16 n <= 156, u32 n <= 564, u64 n <= 2150,
 *            u128 n <= 8520, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *T = NULL;          /* 2‑D table stored linearly */
static size_t stride = 0;        /* = current_max_n + 1 */
static idx_t  max_prepared = 0;  /* how many rows have been filled */

/* retrieve T[n][k] (returns 0 if k<1 or k>n) */
static inline seq_t getT(idx_t n, idx_t k)
{
    if (k < 1 || k > n || n > max_prepared) return 0;
    return T[n * stride + k];
}

/* ensure that rows up to 'need' are computed and stored in T */
static void prepare(idx_t need)
{
    if (need <= max_prepared) return;

    /* allocate a new table of the larger size */
    idx_t    old_max   = max_prepared;
    size_t   old_stride = (size_t)(old_max + 1);
    size_t   new_stride = (size_t)(need + 1);
    size_t   new_size   = new_stride * new_stride;

    seq_t *new_T = (seq_t *)calloc(new_size, sizeof(seq_t)); /* zeroed */
    if (!new_T) abort();      /* out of memory – let the program fail cleanly */

    /* copy already computed values */
    if (T) {
        for (idx_t n = 0; n <= old_max; ++n) {
            size_t base_old = n * old_stride;
            size_t base_new = n * new_stride;
            for (idx_t k = 0; k <= n; ++k)
                new_T[base_new + k] = T[base_old + k];
        }
        free(T);
    }
    T = new_T;
    stride = new_stride;
    max_prepared = need;

    /* compute new rows */
    for (idx_t n = old_max + 1; n <= need; ++n) {
        size_t base = n * stride;
        T[base + 1] = (seq_t)2;   /* T(n,1) = 2 */
        T[base + n] = (seq_t)2;   /* T(n,n) = 2 */

        if (n >= 2) {
            for (idx_t k = 2; k < n; ++k) {
                seq_t val = getT(n-1, k);          /* T(n-1,k) */
                val += getT(n-1, k);               /* 2 * T(n-1,k) */
                val += getT(n-1, k-1);             /* + T(n-1,k-1) */
                val -= getT(n-2, k-1);             /* - 2 * T(n-2,k-1) */
                val -= getT(n-2, k-1);
                if (n >= k + 1)                    /* + T(n-k,k-1) */
                    val += getT(n-k, k-1);
                if (n >= k + 2)                    /* - T(n-k-1,k) */
                    val -= getT(n-k-1, k);
                T[base + k] = val;
            }
        }
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* offset is 1; values below not required */

    /* determine row r and column c via a simple loop */
    idx_t cumulative = 0;     /* sum of lengths of previous rows */
    idx_t r = 1;
    for (;;) {
        if (cumulative + r >= n) break;
        cumulative += r;
        ++r;
    }
    idx_t c = n - cumulative; /* 1‑based column index within row r */

    /* make sure the table contains row r */
    prepare(r);

    return getT(r, c);
}
