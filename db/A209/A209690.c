/*
 * A209690  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209689; see the Formula section.
 * https://oeis.org/A209690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for A209690:
   Let u(1,x)=1, v(1,x)=1.
   For n>1:
     u(n,x)= x*(u(n-1,x)+v(n-1,x))
     v(n,x)= u(n-1,x)+x*v(n-1,x)+1
   The sequence a(n) is the flattened list of the coefficients of v(n,x)
   (row by row, left to right, with a(1)=1).  The recurrence above yields
   the triangle shown in the OEIS entry.
*/

#include <stdlib.h>

static seq_t *u = NULL;          /* coefficients of u(n-1,x)   */
static seq_t *v = NULL;          /* coefficients of v(n-1,x)   */
static size_t u_len = 0;         /* current length of u        */
static size_t v_len = 0;         /* current length of v        */

static seq_t *flat = NULL;       /* flattened coefficients     */
static size_t flat_len = 0;      /* number of terms stored     */
static size_t flat_cap = 0;      /* capacity of flat array     */

/* Ensure the flat array can hold at least `need` elements. */
static void ensure_flat_capacity(size_t need)
{
    if (need <= flat_cap)
        return;
    size_t new_cap = (flat_cap ? flat_cap : 8);
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(flat, new_cap * sizeof(seq_t));
    /* If realloc fails, memory is left untouched; the program will
       terminate on a subsequent access – acceptable for contest use. */
    flat = tmp;
    flat_cap = new_cap;
}

/* Generate the next row of the triangle (row n, where n = current row + 1). */
static void generate_next_row(void)
{
    /* new u has length u_len + 1 */
    size_t new_u_len = u_len + 1;
    seq_t *new_u = (seq_t*)malloc(new_u_len * sizeof(seq_t));
    new_u[0] = 0;
    for (size_t i = 1; i < new_u_len; ++i)
        new_u[i] = u[i-1] + v[i-1];

    /* new v has length v_len + 1 */
    size_t new_v_len = v_len + 1;
    seq_t *new_v = (seq_t*)malloc(new_v_len * sizeof(seq_t));
    new_v[0] = u[0] + 1;
    for (size_t i = 1; i < new_v_len; ++i) {
        seq_t uv = (i < u_len) ? u[i] : 0;
        new_v[i] = uv + v[i-1];
    }

    /* Append new_v to the flattened array */
    ensure_flat_capacity(flat_len + new_v_len);
    for (size_t i = 0; i < new_v_len; ++i)
        flat[flat_len + i] = new_v[i];
    flat_len += new_v_len;

    /* Replace old arrays with the new ones */
    free(u);
    free(v);
    u = new_u;
    v = new_v;
    u_len = new_u_len;
    v_len = new_v_len;
}

/* Initialise the triangle with the first row (n = 1). */
static void init_first_row(void)
{
    if (v != NULL)
        return;                   /* already initialised */
    u = (seq_t*)malloc(sizeof(seq_t));
    v = (seq_t*)malloc(sizeof(seq_t));
    u[0] = 1;
    v[0] = 1;
    u_len = v_len = 1;

    /* Append the first row to the flat array */
    ensure_flat_capacity(1);
    flat[0] = v[0];
    flat_len = 1;
}

/* Public function: return a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    init_first_row();

    /* Generate rows until we have enough terms */
    while (flat_len < (size_t)n)
        generate_next_row();

    return flat[n-1];
}
