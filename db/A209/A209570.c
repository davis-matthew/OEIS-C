/*
 * A209570  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209569; see the Formula section.
 * https://oeis.org/A209570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence defining the triangle:  
   u(1,x)=1, v(1,x)=1;  
   u(n,x)=x*u(n-1,x)+v(n-1,x);  
   v(n,x)=2x*u(n-1,x)+v(n-1,x)+1;  
   a(n) is the flattened list of v coefficients (row‑major). */

#include <stdlib.h>

/* Flattened v‑coefficients, indices 0…(n‑1) for a(n). */
static seq_t *flat  = NULL;
static size_t flat_cap = 0;
static size_t flat_len = 0;

/* Last computed row of u and v. */
static seq_t *prev_u = NULL;
static seq_t *prev_v = NULL;
static size_t prev_len = 0;

/* Grow the tables so that flat[n‑1] exists. */
static void ensure_n(idx_t n) {
    size_t needed = (size_t)n;
    while (flat_len < needed) {
        size_t len = prev_len + 1;               /* new row length */
        seq_t *cur_u = (seq_t *)malloc(len * sizeof(seq_t));
        seq_t *cur_v = (seq_t *)malloc(len * sizeof(seq_t));
        if (!cur_u || !cur_v) abort();

        if (len == 1) {                           /* n = 1 */
            cur_u[0] = (seq_t)1;
            cur_v[0] = (seq_t)1;
        } else {
            for (size_t k = 0; k < len; ++k) {
                seq_t ux = (k > 0 ? prev_u[k - 1] : (seq_t)0);
                seq_t vx = (k < prev_len ? prev_v[k] : (seq_t)0);
                cur_u[k] = ux + vx;
                seq_t vval = (seq_t)2 * ux + vx + ((k == 0) ? (seq_t)1 : (seq_t)0);
                cur_v[k] = vval;
            }
        }

        if (flat_len + len > flat_cap) {
            size_t newcap = flat_cap ? flat_cap * 2 : 64;
            while (flat_len + len > newcap) newcap *= 2;
            seq_t *tmp = (seq_t *)realloc(flat, newcap * sizeof(seq_t));
            if (!tmp) abort();
            flat = tmp;
            flat_cap = newcap;
        }

        for (size_t i = 0; i < len; ++i)
            flat[flat_len + i] = cur_v[i];
        flat_len += len;

        free(prev_u);
        free(prev_v);
        prev_u = cur_u;
        prev_v = cur_v;
        prev_len = len;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* defensive for offset below 1 */
    ensure_n(n);
    return flat[(size_t)(n - 1)];
}
