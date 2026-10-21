/*
 * A097883  Triangle read by rows where T(0,0) is 1, T(m,n) = smallest
 *          as-yet-unused (reading rows from left to right) positive integer
 *          which is coprime to both T(m-1,n) and T(m-1,n-1) (or is coprime to
 *          T(m-1,0) if n=0, or to T(m-1,m-1) if n=m).
 * https://oeis.org/A097883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 5049, u32 n <= 5049, u64 n <= 5049,
 *            u128 n <= 5049, big any n
 * Helpers:   gcd, idx_from_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is read from a triangle T(m,k) computed by
 * inserting the smallest unused positive integer that is coprime to
 * the two upper neighbours (or the single neighbour on the edges).
 * The sequence is the usual row‑by‑row flattening: a(0)=1,
 * a(1)=2, a(2)=3, a(3)=5, …
 *
 * The function stores the triangle and a bitmap of used numbers in
 * static variables (memoised) so that subsequent calls can be answered
 * in O(1) after the required rows have been generated.  It works for
 * the usual built‑in integer types and for the arbitrary‑precision
 * seq_t supplied by the build.  The dynamic allocations use C‑style
 * casts so the file compiles as both C11 and C++17.
 */

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/idx_from_coords.h"

/* static cached state ---------------------------------------------------- */

static seq_t *triangle_data = NULL;          /* flattened triangle values   */
static size_t  triangle_cap  = 0;            /* capacity of triangle_data   */
static idx_t   triangle_max_row = 0;         /* max row computed (0‑based)  */

static unsigned char *used_flags = NULL;    /* bitmap of used integers    */
static size_t  used_cap   = 0;              /* capacity of used_flags     */
static seq_t   smallest_unused = 2;         /* next candidate integer     */

/* ---------------------------------------------------------------------- */

static void ensure_used_capacity(idx_t needed)
{
    if ((size_t)needed < used_cap)
        return;
    size_t new_cap = used_cap;
    if (new_cap == 0) new_cap = 4;
    while (new_cap <= (size_t)needed)
        new_cap <<= 1;
    unsigned char *tmp = (unsigned char*)realloc(used_flags, new_cap);
    if (!tmp) abort();
    /* zero the added region */
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used_flags = tmp;
    used_cap   = new_cap;
}

static void ensure_triangle_capacity(size_t needed)
{
    if (triangle_cap >= needed)
        return;
    size_t new_cap = triangle_cap;
    if (new_cap == 0) new_cap = 4;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(triangle_data, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    triangle_data = tmp;
    triangle_cap  = new_cap;
}

/* ---------------------------------------------------------------------- */

static void init_state(void)
{
    if (triangle_data) /* already initialised */
        return;
    /* initialise row 0 */
    triangle_cap  = 4;
    triangle_data = (seq_t*)calloc(triangle_cap, sizeof(seq_t));
    if (!triangle_data) abort();
    triangle_data[0] = 1;
    triangle_max_row = 0;

    used_cap   = 4;
    used_flags = (unsigned char*)calloc(used_cap, 1);
    if (!used_flags) abort();
    used_flags[1] = 1;            /* 1 is used by T(0,0) */
    smallest_unused = 2;          /* next candidate */
}

/* ---------------------------------------------------------------------- */

static void compute_row(idx_t m)
{
    /* reserve space for all rows up to m */
    size_t needed = ((size_t)m + 1) * ((size_t)m + 2) / 2;
    ensure_triangle_capacity(needed);

    for (idx_t n = 0; n <= m; ++n) {
        seq_t left_val  = 0, right_val = 0;
        bool use_left  = false, use_right = false;

        if (n == 0) {                     /* first element */
            left_val = triangle_data[idx_from_coords(m - 1, 0)];
            use_left = true;
        } else if (n == m) {               /* last element */
            left_val = triangle_data[idx_from_coords(m - 1, m - 1)];
            use_left = true;
        } else {                           /* interior element */
            left_val  = triangle_data[idx_from_coords(m - 1, n)];
            right_val = triangle_data[idx_from_coords(m - 1, n - 1)];
            use_left = use_right = true;
        }

        /* find smallest unused integer coprime to the used neighbours */
        seq_t cur = smallest_unused;
        for (;;) {
            ensure_used_capacity((idx_t)cur);
            if (used_flags[(size_t)cur])
                ++cur;                     /* already used */
            else if (use_left && gcd(cur, left_val) != 1)
                ++cur;                     /* not coprime to left */
            else if (use_right && gcd(cur, right_val) != 1)
                ++cur;                     /* not coprime to right */
            else
                break;                     /* suitable number */
        }

        /* store and mark used */
        size_t pos = idx_from_coords(m, n);
        triangle_data[pos] = cur;
        used_flags[(size_t)cur] = 1;

        /* advance smallest_unused if it was used */
        if (cur == smallest_unused) {
            ++smallest_unused;
            while (used_flags[(size_t)smallest_unused])
                ++smallest_unused;
        }
    }
    triangle_max_row = m;
}

/* ---------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    init_state();

    /* determine row containing nth element */
    idx_t row = 0;
    while (((size_t)row + 1) * ((size_t)row + 2) / 2 <= n)
        ++row;

    /* generate rows up to that row */
    while (row > triangle_max_row)
        compute_row(++triangle_max_row);

    idx_t col = n - ((size_t)row * (row + 1)) / 2;
    return triangle_data[idx_from_coords(row, col)];
}
