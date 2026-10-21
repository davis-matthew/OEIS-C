/*
 * A208333  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208332; see the Formula section.
 * https://oeis.org/A208333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value of the triangle T(n-1, k) listed row‑wise where
 *            T(n,k) is defined by T(n,k) = T(n-1,k) + 2*T(n-1,k-1) -
 *            2*T(n-2,k-1) + 2*T(n-2,k-2), T(0,0)=1, T(1,0)=0, T(1,1)=4,
 *            T(n,k)=0 if k<0 or k>n
 * Range:     u8 n <= 26, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* integer square root for 128‑bit numbers (returns floor(sqrt(x))) */
static inline unsigned long long isqrt_u128(__int128 x) {
    unsigned long long low = 0, high = 1ULL << 35;   /* > sqrt(2^67) */
    while (low < high) {
        unsigned long long mid = (low + high + 1) >> 1;
        if ((__int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* static cache of computed rows of the triangle */
static seq_t **triangle = NULL;   /* triangle[row] = pointer to row array */
static size_t  triangle_cap = 0;  /* number of allocated row pointers   */
static idx_t   triangle_max = 0;  /* highest row index that has been computed */

/* Ensure that rows 0 … r are computed and stored */
static void ensure_rows(idx_t r) {
    if (triangle && r <= triangle_max)
        return;                /* nothing to do */

    size_t new_cap = (size_t)(r + 1);
    seq_t **new_tri = (seq_t **)(malloc(new_cap * sizeof(seq_t *)));
    if (!new_tri)
        exit(1);               /* out of memory */

    /* copy old row pointers */
    for (size_t i = 0; i < triangle_cap; ++i)
        new_tri[i] = triangle[i];

    /* compute missing rows from triangle_cap up to r */
    for (idx_t i = (idx_t)triangle_cap; i <= r; ++i) {
        seq_t *row = (seq_t *)malloc((size_t)(i + 1) * sizeof(seq_t));
        if (!row)
            exit(1);
        new_tri[(size_t)i] = row;

        if (i == 0) {               /* base row 0 */
            row[0] = (seq_t)1;
        } else if (i == 1) {        /* base row 1 */
            row[0] = (seq_t)0;
            row[1] = (seq_t)4;
        } else {                    /* recurrence for i >= 2 */
            seq_t *prev  = new_tri[(size_t)(i - 1)];
            seq_t *prev2 = new_tri[(size_t)(i - 2)];
            /* k = 0 */
            row[0] = prev[0];
            for (idx_t k = 1; k <= i; ++k) {
                seq_t a = (k <= (i - 1)) ? prev[(size_t)k] : (seq_t)0;
                seq_t b = prev[(size_t)(k - 1)];
                seq_t c = ((k - 1) <= (i - 2)) ? prev2[(size_t)(k - 1)] : (seq_t)0;
                seq_t d = ((k - 2) <= (i - 2)) ? prev2[(size_t)(k - 2)] : (seq_t)0;

                seq_t val = a;
                val += b; val += b;      /* +2*b */
                seq_t temp = c; temp += c;   /* 2*c */
                if (val >= temp)
                    val -= temp;
                else
                    val = (seq_t)0;
                seq_t temp2 = d; temp2 += d;   /* +2*d */
                val += temp2;
                row[(size_t)k] = val;
            }
        }
    }

    /* replace old array of row pointers with the new one */
    free(triangle);                 /* rows themselves remain allocated */
    triangle       = new_tri;
    triangle_cap   = new_cap;
    triangle_max   = r;
}

/* Return the n‑th term of sequence A208333 (offset 1). */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;            /* safety for out‑of‑range input */

    /* Convert to zero‑based index. */
    idx_t m = n - 1;                /* linear position in the triangle */

    /* Find r such that r(r+1)/2 <= m < (r+1)(r+2)/2. */
    __int128 val = (__int128)8 * m + 1;
    unsigned long long s = isqrt_u128(val);           /* floor sqrt(8*m+1) */
    idx_t r = (idx_t)(s - 1) / 2;

    __int128 tri = (__int128)r * (r + 1) / 2;
    idx_t k = m - (idx_t)tri;                /* column within the row */

    /* Ensure required rows are computed. */
    ensure_rows(r);

    return triangle[(size_t)r][(size_t)k];
}
