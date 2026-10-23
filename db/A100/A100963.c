/*
 * A100963  a(1)=1. a(n) = a(n-1) + sum of the triangular numbers which are
 *          among the first (n-1) terms of the sequence.
 * https://oeis.org/A100963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n)=a(n-1)+sum of the triangular numbers among
 *            a(1)…a(n-1)
 * Range:     u8 n <= 11, u16 n <= 83, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* --- helper: generate triangular numbers up to a limit ----------------- */
static seq_t *tri_arr      = NULL; /* list of triangular numbers */
static idx_t   tri_cnt     = 0;    /* number of triangular numbers stored */
static idx_t   tri_cap     = 0;    /* allocated capacity of tri_arr */
static seq_t   last_tri    = 0;    /* last triangular number generated */
static idx_t   next_k      = 1;    /* next k to generate T_k */

/* extend the triangular list so that last_tri >= limit */
static void extend_tri_to(seq_t limit)
{
    while (last_tri < limit) {
        seq_t next_tri = last_tri + (seq_t)next_k;

        if (tri_cnt == tri_cap) {
            idx_t new_cap = (tri_cap == 0) ? 4 : tri_cap << 1;
            seq_t *tmp = (seq_t *)realloc(tri_arr, new_cap * sizeof(seq_t));
            tri_arr    = tmp;
            tri_cap    = new_cap;
        }

        tri_arr[tri_cnt++] = next_tri;
        last_tri   = next_tri;
        next_k++;
    }
}

/* test whether x is a triangular number */
static bool is_triangular(seq_t x)
{
    extend_tri_to(x);
    for (idx_t i = 0; i < tri_cnt; ++i) {
        if (tri_arr[i] == x)
            return true;
    }
    return false;
}

/* --- main sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n<1 */

    /* static storage for computed terms and bookkeeping */
    static seq_t *a_arr       = NULL;   /* a[1..computed] */
    static idx_t   a_size     = 0;      /* allocated size (in elements) */
    static idx_t   computed   = 0;      /* maximum n computed so far */
    static seq_t   cumulative = 0;      /* sum of triangular numbers seen so far */

    /* grow the array if necessary */
    if (n + 1 > a_size) {
        idx_t new_size = (a_size == 0) ? 2 : a_size << 1;
        while (new_size <= n + 1) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_arr, new_size * sizeof(seq_t));
        a_arr = tmp;
        a_size = new_size;
        /* new elements are zero-initialised by realloc memory contract on
           GNU/C standard compliant allocators; if not, they can be set to 0
           here as needed. */
    }

    /* initialise first value if necessary */
    if (computed == 0) {
        a_arr[1] = 1;
        extend_tri_to(1);                    /* generate triangular numbers up to 1 */
        cumulative = is_triangular(1) ? (seq_t)1 : (seq_t)0;
        computed   = 1;
    }

    /* compute terms up to n if required */
    for (idx_t i = computed + 1; i <= n; ++i) {
        a_arr[i] = a_arr[i - 1] + cumulative;  /* recurrence value */

        /* update cumulative sum if a[i] itself is triangular */
        if (is_triangular(a_arr[i]))
            cumulative += a_arr[i];
    }
    computed = n;

    return a_arr[n];
}
