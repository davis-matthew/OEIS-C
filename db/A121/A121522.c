/*
 * A121522  Triangle read by rows: T(n,k) is the number of nondecreasing Dyck
 *          paths of semilength n and having k up steps starting at an even
 *          level (1<=k<=n). A nondecreasing Dyck path is a Dyck path for
 *          which the sequence of the altitudes of the valleys is
 *          nondecreasing.
 * https://oeis.org/A121522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (row‑wise) entry T(r,k) of the triangle A121522.
 *            T(n,k) is computed from the generating‑function recurrence
 *            G_n(t) = N_n(t) + (1 + t) G_{n-1}(t) + 4 t G_{n-2}(t) - 2 t
 *            G_{n-3}(t) - 2 t^2 G_{n-3}(t) - 6 t^2 G_{n-4}(t) + t^3
 *            G_{n-6}(t) where G_n(t)=∑_{k≥1}T(n,k)t^k and the polynomials
 *            N...
 * Range:     u8 n <= 39, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* multiply a seq_t value by a small unsigned integer in a
   portable way (works for builtin integral seq_t and arbitrary‑precision). */
static inline seq_t mul_uint(seq_t v, unsigned int m)
{
    if (sizeof(seq_t) <= 8) { /* builtin integral case */
        return (seq_t)(((__int128)v) * m);
    } else {                  /* arbitrary‑precision case – built‑in * works */
        return v * (seq_t)m;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* offset = 1 */

    /* --------------------------------- compute row r --------------------------------- */
    idx_t r = 1;
    /* find smallest r with r(r+1)/2 >= n */
    while ((idx_t)(r * (r + 1) / 2) < n) r++;

    /* column index within row (1‑based) */
    idx_t prev_sum = (idx_t)((r - 1) * r / 2);
    idx_t col = n - prev_sum;          /* 1 ≤ col ≤ r */

    /* --------------------------------- allocate circular buffer --------------------------------- */
    seq_t *rows[7] = {0};
    rows[0] = (seq_t*)calloc(1, sizeof(seq_t));   /* G_0(t) = 0 */
    if (!rows[0]) return 0;                      /* out of memory */

    /* --------------------------------- build rows up to r --------------------------------- */
    for (idx_t i = 1; i <= r; ++i)
    {
        size_t idx = (size_t)(i % 7);
        /* free old row in the slot (except row 0 which is special) */
        if (rows[idx]) free(rows[idx]);

        /* current and temporary negative buffers */
        seq_t *cur  = (seq_t*)calloc((size_t)(i + 1), sizeof(seq_t));
        seq_t *sub  = (seq_t*)calloc((size_t)(i + 1), sizeof(seq_t));
        if (!cur || !sub) return 0;          /* out of memory */

        /* ------------ numerator terms N_i(t) ------------ */
        switch (i) {
            case 1: cur[1] = 1;                break;
            case 3: sub[2] = 3;                break;
            case 4: sub[2] = 1;                break;
            case 5: cur[3] = 2;                break;
            case 6: cur[3] = 1;                break;
            default: break;
        }

        /* ------------ contributions from previous rows ------------ */
        for (int j = 1; j <= 6; ++j) {
            if (i < j) continue;           /* previous row does not exist */
            idx_t prev_i = i - j;
            seq_t *prev = rows[(size_t)(prev_i % 7)];
            size_t len_prev = (size_t)(prev_i + 1);

            for (size_t k = 1; k < len_prev; ++k) {
                seq_t val = prev[k];
                if (!val) continue;

                switch (j) {
                    case 1: /* (1 + t) * G_{i-1}  */
                        cur[k] += val;
                        if (k + 1 <= i) cur[k + 1] += val;
                        break;
                    case 2: /* 4t * G_{i-2}   */
                        if (k + 1 <= i)
                            cur[k + 1] += mul_uint(val, 4);
                        break;
                    case 3: /* -2t  - 2t² * G_{i-3}  */
                        if (k + 1 <= i) sub[k + 1] += mul_uint(val, 2);
                        if (k + 2 <= i) sub[k + 2] += mul_uint(val, 2);
                        break;
                    case 4: /* -6t² * G_{i-4} */
                        if (k + 2 <= i) sub[k + 2] += mul_uint(val, 6);
                        break;
                    case 6: /* +t³ * G_{i-6} */
                        if (k + 3 <= i) cur[k + 3] += val;
                        break;
                    default: /* j==5 has no term */ break;
                }
            }
        }

        /* ------------ final subtraction of negative parts ------------ */
        for (size_t k = 0; k <= i; ++k)
            if (sub[k]) cur[k] -= sub[k];

        /* store current row in the circular buffer */
        rows[idx] = cur;
        free(sub);
    }

    /* --------------------------------- fetch answer --------------------------------- */
    seq_t result = rows[(size_t)(r % 7)][(size_t)col];

    /* --------------------------------- clean up --------------------------------- */
    for (int j = 0; j < 7; ++j)
        if (rows[j]) free(rows[j]);

    return result;
}
