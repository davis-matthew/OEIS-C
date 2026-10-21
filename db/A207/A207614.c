/*
 * A207614  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A207615; see the Formula section.
 * https://oeis.org/A207614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A207614: Triangle of coefficients of polynomials u(n,x)
   Recurrence:
      u(1,x) = 1
      v(1,x) = 1
      u(n,x) = u(n‑1,x) + v(n‑1,x)
      v(n,x) = u(n‑1,x) + (x+1)·v(n‑1,x) + 1
   The sequence is obtained by listing, for each n ≥ 2,
   the coefficients of u(n,x) from the constant term up to the leading
   coefficient.  For n = 1 the single term is 1.  The indexing follows
   OEIS: a(1)=1, a(2)=2, a(3)=5, a(4)=1, …   */
#include <stdlib.h>

#if defined(__cplusplus)
/* use C++ new/delete for class types, C malloc/free for C */
#define ALLOC_T(T, N)  new T[(N)]
#define FREE_T(P)      delete[] (P)
#else
#define ALLOC_T(T, N)  ((T*)malloc(sizeof(T)*(N)))
#define FREE_T(P)      free((P))
#endif

seq_t a(idx_t n)
{
    /* Special case n == 1 */
    if (n == 1) return (seq_t)1;

    /* Find row r (≥2) and position pos (1‑based within the row)
       The number of terms up to row r is 1 + (r‑1)·r/2  */
    idx_t r = 2;
    idx_t cum = 1;            /* terms up to row 1 */
    while (1) {
        cum += (r - 1);       /* add terms of row r */
        if (cum >= n) break;
        ++r;
    }
    idx_t pos = n - (cum - (r - 1));   /* position within row r, 1‑based */

    /* Build the recurrence up to row r.
       u_len = length of u array for the current row
       v_len = length of v array for the current row   */
    size_t u_len = 1;   /* row 1: u(1,x) has one coefficient */
    size_t v_len = 1;   /* row 1: v(1,x) has one coefficient */
    seq_t *u = ALLOC_T(seq_t, u_len);
    seq_t *v = ALLOC_T(seq_t, v_len);
    if (!u || !v) return (seq_t)0;   /* allocation error */
    u[0] = (seq_t)1;
    v[0] = (seq_t)1;

    /* Iteratively compute rows 2 … r */
    for (size_t cur = 2; cur <= (size_t)r; ++cur) {
        size_t new_u_len = cur - 1;   /* u(n) has degree n‑1, so length n‑1 */
        size_t new_v_len = cur;       /* v(n) has degree n‑1, so length n   */

        seq_t *new_u = ALLOC_T(seq_t, new_u_len);
        seq_t *new_v = ALLOC_T(seq_t, new_v_len);
        if (!new_u || !new_v) {
            FREE_T(u);
            FREE_T(v);
            FREE_T(new_u);
            FREE_T(new_v);
            return (seq_t)0;   /* allocation error */
        }

        /* u(n) = u(n‑1) + v(n‑1) */
        for (size_t d = 0; d < new_u_len; ++d) {
            seq_t part_u = (d < u_len) ? u[d] : (seq_t)0;
            seq_t part_v = (d < v_len) ? v[d] : (seq_t)0;
            new_u[d] = part_u + part_v;
        }

        /* v(n) = u(n‑1) + (x+1)·v(n‑1) + 1 */
        for (size_t d = 0; d < new_v_len; ++d) {
            seq_t part_u = (d < u_len) ? u[d] : (seq_t)0;
            seq_t part_v = (d < v_len) ? v[d] : (seq_t)0;
            seq_t shift = (d > 0 && (d - 1) < v_len) ? v[d - 1] : (seq_t)0;
            if (d == 0)
                new_v[d] = part_u + part_v + (seq_t)1;
            else
                new_v[d] = part_u + part_v + shift;
        }

        /* Prepare for next iteration */
        FREE_T(u);
        FREE_T(v);
        u = new_u;
        v = new_v;
        u_len = new_u_len;
        v_len = new_v_len;
    }

    /* Retrieve the requested coefficient from u(r,x) */
    seq_t result = u[pos - 1];

    /* Clean up */
    FREE_T(u);
    FREE_T(v);

    return result;
}
