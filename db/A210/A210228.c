/*
 * A210228  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210227; see the Formula section.
 * https://oeis.org/A210228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The sequence a(n) is the flattened coefficients of the *
 *            polynomials v(n,x) defined jointly with u(n,x) by * * u(1,x) =
 *            1, v(1,x) = 1 * u(n,x) = x*u(n-1,x)+v(n-1,x)+1 (n>1) * v(n,x) =
 *            2x*u(n-1,x)+2x*v(n-1,x)+1 (n>1) * * The triangle is read row by
 *            row from u(1,x) to u(n,x); the * coefficients of...
 * Range:     u8 n <= 25, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#if defined(__cplusplus)
#include <vector>
#endif

/* Ensure absolute safety in 128‑bit intermediates. */
typedef unsigned __int128 u128;

/* Return the row r (1‑based) that contains the n‑th term. */
static inline size_t row_for_index(idx_t n)
{
    size_t lo = 1;
    size_t hi = (size_t)n;      /* r <= n for all n>=1 */

    while (lo < hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        u128 t = (u128)mid * (mid + 1) / 2;
        if (t >= (u128)n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* C part of the implementation. */
#if !defined(__cplusplus)

static seq_t *u_arr = NULL;
static seq_t *v_arr = NULL;
static seq_t *a_values = NULL;
static size_t cur_row = 0;     /* rows computed so far (0 means none) */
static size_t a_len   = 0;     /* number of a(n) values stored */
static size_t a_cap   = 0;     /* capacity of a_values */

static inline void ensure_a_capacity(size_t needed)
{
    if (a_len + needed <= a_cap)
        return;
    size_t new_cap = a_cap ? a_cap : 64;
    while (a_len + needed > new_cap)
        new_cap *= 2;
    seq_t *tmp = realloc(a_values, new_cap * sizeof(seq_t));
    if (!tmp) { /* out of memory – return 0, caller sees it as 0 */
        free(u_arr);
        free(v_arr);
        exit(1);
    }
    a_values = tmp;
    a_cap    = new_cap;
}

seq_t a(idx_t n)
{
    size_t required_row = row_for_index(n);

    if (required_row > cur_row) {
        for (size_t r = cur_row + 1; r <= required_row; ++r) {
            size_t new_len = r;
            seq_t *u_new = malloc(new_len * sizeof(seq_t));
            seq_t *v_new = malloc(new_len * sizeof(seq_t));
            if (!u_new || !v_new) {
                free(u_new);
                free(v_new);
                free(u_arr);
                free(v_arr);
                exit(1);
            }

            if (r == 1) {          /* base case */
                u_new[0] = (seq_t)1;
                v_new[0] = (seq_t)1;
            } else {
                /* v_new[0] = 1 for all n>=2 */
                v_new[0] = (seq_t)1;
                for (size_t k = 1; k < new_len; ++k) {
                    seq_t u_prev = u_arr[k - 1];
                    seq_t v_prev = v_arr[k - 1];
                    v_new[k] = (seq_t)2 * u_prev + (seq_t)2 * v_prev;
                }
                /* u_new[0] = v_prev[0] + 1 = 2 for n>=2 */
                u_new[0] = v_arr[0] + (seq_t)1;
                for (size_t k = 1; k < new_len; ++k) {
                    seq_t u_prev = u_arr[k - 1];
                    seq_t v_prev_k = (k < cur_row) ? v_arr[k] : (seq_t)0;
                    u_new[k] = u_prev + v_prev_k;
                }
            }

            /* append v_new coefficients to a_values */
            ensure_a_capacity(new_len);
            memcpy(a_values + a_len, v_new, new_len * sizeof(seq_t));
            a_len += new_len;

            /* free old arrays and promote the new ones */
            free(u_arr);
            free(v_arr);
            u_arr = u_new;
            v_arr = v_new;
            cur_row = new_len;
        }
    }

    /* index in a_values is 0‑based but n is 1‑based */
    return a_values[n - 1];
}

#else /* __cplusplus */

static std::vector<seq_t> u_arr;
static std::vector<seq_t> v_arr;
static std::vector<seq_t> a_values;
static size_t cur_row = 0;     /* rows computed so far (0 means none) */

seq_t a(idx_t n)
{
    size_t required_row = row_for_index(n);

    for (size_t r = cur_row + 1; r <= required_row; ++r) {
        size_t new_len = r;
        std::vector<seq_t> u_new(new_len);
        std::vector<seq_t> v_new(new_len);

        if (r == 1) {          /* base case */
            u_new[0] = (seq_t)1;
            v_new[0] = (seq_t)1;
        } else {
            v_new[0] = (seq_t)1;
            for (size_t k = 1; k < new_len; ++k) {
                seq_t u_prev = u_arr[k - 1];
                seq_t v_prev = v_arr[k - 1];
                v_new[k] = (seq_t)2 * u_prev + (seq_t)2 * v_prev;
            }
            u_new[0] = v_arr[0] + (seq_t)1;
            for (size_t k = 1; k < new_len; ++k) {
                seq_t u_prev = u_arr[k - 1];
                seq_t v_prev_k = (k < u_arr.size()) ? v_arr[k] : (seq_t)0;
                u_new[k] = u_prev + v_prev_k;
            }
        }

        /* Append coefficients to the flattened array. */
        a_values.insert(a_values.end(), v_new.begin(), v_new.end());

        /* Prepare for the next iteration. */
        u_arr.swap(u_new);
        v_arr.swap(v_new);
        cur_row = new_len;
    }

    return a_values[n - 1];
}

#endif
