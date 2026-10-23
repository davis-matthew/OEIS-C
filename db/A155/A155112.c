/*
 * A155112  Triangle T(n,k), 0<=k<=n, read by rows given by
 *          [0,2,-1/2,-1/2,0,0,0,0,0,0,0,0,...] DELTA [1,0,0,0,0,0,0,0,0,...]
 *          where DELTA is the operator defined in A084938.
 * https://oeis.org/A155112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{i=0..floor(n/2)} C(n-i,i)*C(n-i,k)*k/(n-i)
 * Range:     u8 n <= 38, u16 n <= 109, u32 n <= 333, u64 n <= 1191,
 *            u128 n <= 4310, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the flattened row‑by‑row reading of the triangle T(n,k). */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
/* C++ allocator – constructs default values */
static seq_t* alloc_seq_array(size_t n) { return new seq_t[n](); }
static void  dealloc_seq_array(seq_t *p)       { delete [] p; }
/* reallocate with zero‑initialisation of new elements */
static seq_t* realloc_seq_array(seq_t *old, size_t oldlen, size_t newlen) {
    seq_t *newp = new seq_t[newlen]();
    for (size_t i = 0; i < oldlen; ++i) newp[i] = old[i];
    delete [] old;
    return newp;
}
#else
/* C allocator – no construction needed for POD types */
static seq_t* alloc_seq_array(size_t n) { return (seq_t*)malloc(n * sizeof(seq_t)); }
static void  dealloc_seq_array(seq_t *p) { free(p); }
static seq_t* realloc_seq_array(seq_t *old, size_t oldlen, size_t newlen) {
    seq_t *newp = (seq_t*)realloc(old, newlen * sizeof(seq_t));
    /* new elements are uninitialised, but seq_t is POD in this build */
    return newp;
}
#endif

seq_t a(idx_t n)
{
    /* static state: the flattened sequence, and the last two rows of T */
    static seq_t *flat     = NULL;   /* flattened stream of all entries */
    static size_t flat_len = 0;      /* number of entries stored */
    static size_t flat_cap = 0;      /* capacity of flat              */

    static seq_t *prev1    = NULL;   /* row n-1                      */
    static seq_t *prev2    = NULL;   /* row n-2                      */
    static size_t prev1_len = 0;     /* length of prev1              */
    static size_t prev2_len = 0;     /* length of prev2              */

    static int  last_row   = -1;      /* index of last built row     */

    /* return cached value if already known */
    if ((size_t)n < flat_len)
        return flat[(size_t)n];

    /* initialise on first call */
    if (last_row < 0) {
        /* row 0 : [1] */
        prev2_len = 1;
        prev2 = alloc_seq_array(prev2_len);
        prev2[0] = (seq_t)1;

        /* ensure flat capacity for row 0 */
        if (flat_cap < 1) {
            flat_cap = 1;
            flat = alloc_seq_array(flat_cap);
        }
        flat[flat_len++] = (seq_t)1;   /* a(0) */

        /* row 1 : [0,1] */
        last_row = 1;
        prev1_len = 2;
        prev1 = alloc_seq_array(prev1_len);
        prev1[0] = (seq_t)0;
        prev1[1] = (seq_t)1;

        /* ensure flat capacity for row 1 */
        if (flat_cap < flat_len + 2) {
            flat_cap = flat_len + 2;
            flat = realloc_seq_array(flat, flat_len, flat_cap);
        }
        flat[flat_len++] = prev1[0];
        flat[flat_len++] = prev1[1];
    }

    /* generate rows until we have index n available */
    while (flat_len <= (size_t)n) {
        /* compute next row index */
        int new_row = last_row + 1;          /* row number to build */
        size_t new_len = (size_t)new_row + 1; /* number of elements in that row */

        /* allocate space for the new row */
        seq_t *row_new = alloc_seq_array(new_len);

        /* ensure flat has room for the new elements */
        if (flat_cap < flat_len + new_len) {
            flat_cap = flat_len + new_len;
            flat = realloc_seq_array(flat, flat_len, flat_cap);
        }

        /* fill the new row using the recurrence */
        for (size_t k = 0; k < new_len; ++k) {
            seq_t val = (seq_t)0;
            if (k > 0) {
                if (k < prev1_len)      val += prev1[k];
                if (k-1 < prev1_len)    val += prev1[k-1];
                if (k < prev2_len)      val += prev2[k];
                if (k-1 < prev2_len)    val += prev2[k-1];
            }
            row_new[k] = val;
            flat[flat_len++] = val;
        }

        /* rotate row pointers */
        dealloc_seq_array(prev2);
        prev2 = prev1;
        prev2_len = prev1_len;

        prev1 = row_new;
        prev1_len = new_len;

        last_row = new_row;
    }

    return flat[(size_t)n];
}
