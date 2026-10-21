/*
 * A050039  a(n) = a(n-1) + a(m) for n >= 4, where m = 2*n - 2 - 2^(p+1) and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = a(2) = 1 and a(3) = 4.
 * https://oeis.org/A050039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(m), m = 2*n - 2 - 2^(p+1), * where p is the
 *            unique integer with 2^p < n-1 <= 2^(p+1). * Initial values:
 *            a(1)=1, a(2)=1, a(3)=4. This is OEIS sequence A050039
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 126, u64 n <= 505,
 *            u128 n <= 3082, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_arr = NULL;   /* 1‑based array of computed terms */
static idx_t  seq_cap = 0;      /* allocated array size (index 0..seq_cap-1) */
static idx_t  computed = 0;     /* largest index whose value has been computed */

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;   /* below offset is irrelevant */
    }

    /* Ensure array can address index n (plus dummy 0 element) */
    if (seq_cap < n + 1) {
        seq_t *tmp = (seq_t *)realloc(seq_arr, (n + 1) * sizeof(seq_t));
        if (!tmp) {
            abort();  /* out‑of‑memory */
        }
        seq_arr = tmp;
        seq_cap = n + 1;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1) {
            seq_arr[i] = (seq_t)1;
        } else if (i == 2) {
            seq_arr[i] = (seq_t)1;
        } else if (i == 3) {
            seq_arr[i] = (seq_t)4;
        } else {
            /* i >= 4, so i-2 > 0 */
            int p = 63 - __builtin_clzll((unsigned long long)(i - 2));
            idx_t pow = ((idx_t)1) << (p + 1);   /* 2^(p+1) */
            idx_t m = 2 * i - 2 - pow;           /* 1 <= m < i */
            seq_arr[i] = seq_arr[i - 1] + seq_arr[m];
        }
    }

    if (n > computed) {
        computed = n;
    }

    return seq_arr[n];
}
