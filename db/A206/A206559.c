/*
 * A206559  Number of 9's in the last section of the set of partitions of n.
 * https://oeis.org/A206559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = Σ_{t=0}^{⌊(n-9)/9⌋} [ p(n-9-9t) - p(n-10-9t) ] * where
 *            p(0)=1, p(m<0)=0 and p satisfies the pentagonal number
 *            recurrence
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static storage for partition numbers p[m] and sequence values a[n] */
static seq_t *p_arr  = NULL;      /* p[m] for m = 0..max_n */
static seq_t *a_arr  = NULL;      /* a[n] for n = 0..max_n */
static idx_t  max_n  = 0;          /* highest index computed so far */

static void ensure_arrays(idx_t n)
{
    if (n <= max_n) return;               /* nothing to do */

    idx_t new_n = n;
    size_t new_cap = (size_t)(new_n + 1);

    /* Allocate or extend arrays */
#if defined(__cplusplus)
    /* C++: use new[] to invoke constructors and value‑initialize to zero */
    if (max_n == 0) {
        p_arr = new seq_t[new_cap]();
        a_arr = new seq_t[new_cap]();
        p_arr[0] = 1;                /* p(0) = 1 */
        a_arr[0] = 0;                /* a(0) is unused but initialized */
    } else {
        /* Allocate new bigger arrays */
        seq_t *p_tmp = new seq_t[new_cap]();
        seq_t *a_tmp = new seq_t[new_cap]();
        /* Copy old data */
        for (idx_t i = 0; i <= max_n; ++i) {
            p_tmp[i] = p_arr[i];
            a_tmp[i] = a_arr[i];
        }
        /* Replace old arrays */
        delete[] p_arr;
        delete[] a_arr;
        p_arr = p_tmp;
        a_arr = a_tmp;
    }
#else
    /* C: use malloc/realloc (no constructors needed for plain types) */
    if (max_n == 0) {
        p_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        a_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        p_arr[0] = 1;                /* p(0) = 1 */
        a_arr[0] = 0;                /* a(0) is unused but initialized */
    } else {
        p_arr = (seq_t *)realloc(p_arr, new_cap * sizeof(seq_t));
        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        /* Newly allocated elements are undefined; they will be set later. */
    }
#endif

    /* Compute partition numbers p[m] for m = max_n+1 .. new_n */
    for (idx_t m = max_n + 1; m <= new_n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t gp1 = k * (3 * k - 1) / 2;            /* g(k) */
            if (gp1 > m) break;
            seq_t term = p_arr[m - gp1];
            if (k & 1) sum += term; else sum -= term;

            idx_t gp2 = k * (3 * k + 1) / 2;            /* g(-k) */
            if (gp2 <= m) {
                term = p_arr[m - gp2];
                if (k & 1) sum += term; else sum -= term;
            }
        }
        p_arr[m] = sum;
    }

    /* Compute sequence values a[n] for n = max_n+1 .. new_n */
    for (idx_t idx = max_n + 1; idx <= new_n; ++idx) {
        if (idx < 9) {                      /* a(n) = 0 for n < 9 */
            a_arr[idx] = 0;
            continue;
        }
        seq_t sum = 0;
        idx_t max_t = (idx - 9) / 9;         /* floor((n-9)/9) */
        for (idx_t t = 0; t <= max_t; ++t) {
            idx_t m = idx - 9 - 9 * t;          /* argument for p */
            seq_t term = p_arr[m];
            if (m > 0) term -= p_arr[m - 1];
            sum += term;
        }
        a_arr[idx] = sum;
    }

    max_n = new_n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < offset */
    ensure_arrays(n);
    return a_arr[n];
}
