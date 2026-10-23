/*
 * A141206  Let sequences X and Y consist of the least positive integers such
 *          that 2X+Y is the complement of X and X+Y is the complement of Y,
 *          starting with X(1)=1 and Y(1)=1; then this sequence equals Y,
 *          while X=A141204, 2X+Y=A141205 and X+Y=A141207.
 * https://oeis.org/A141206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 166, u16 n <= 420, u32 n <= 420, u64 n <= 420,
 *            u128 n <= 420, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A141206 – Y sequence from the joint construction of X and Y.  */
/*  The algorithm follows the definition:  */
 /*  * X(n) is the least positive integer not yet appearing in X or in 2X+Y.  */
 /*  * Y(n) is the least positive integer not yet appearing in Y or in X+Y.  */
 /*  * After picking X(n), Y(n) the “complement” elements 2X(n)+Y(n) and X(n)+Y(n)   */
 /*    are marked as used so they cannot appear as future terms of X or Y.  */
 /*  The function returns Y(n).  */

/*  The code satisfies the build constraints: it uses only integer arithmetic,  */
 /*  and works for arbitrary-precision seq_t because indices are always 64‑bit. */

#include <stdlib.h>   /* realloc, abort */
#include <stddef.h>   /* size_t */
#include <string.h>   /* memset */

/*  External type definitions supplied by the build system.  */
extern seq_t a(idx_t n);

/*  Dynamic storage for the X and Y sequences.  */
static seq_t *X_seq = NULL, *Y_seq = NULL;
static size_t X_cap = 0, Y_cap = 0;
static size_t X_len = 0, Y_len = 0;

/*  Boolean flag arrays to mark which numbers are already used.  */
static unsigned char *usedX = NULL, *usedCompX = NULL;
static unsigned char *usedY = NULL, *usedCompY = NULL;
static size_t used_cap = 0;

/*  Ensure that the flag arrays can index up to 'need' (inclusive).  */
static void grow_used_arrays(size_t need)
{
    if (need < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap : 4;
    while (new_cap <= need)
        new_cap <<= 1;
    unsigned char *x = (unsigned char *)realloc(usedX, new_cap);
    unsigned char *c = (unsigned char *)realloc(usedCompX, new_cap);
    unsigned char *y = (unsigned char *)realloc(usedY, new_cap);
    unsigned char *d = (unsigned char *)realloc(usedCompY, new_cap);
    if (!x || !c || !y || !d)
        abort();
    /*  Initialise newly allocated part to zero.  */
    if (new_cap > used_cap) {
        memset(x + used_cap, 0, new_cap - used_cap);
        memset(c + used_cap, 0, new_cap - used_cap);
        memset(y + used_cap, 0, new_cap - used_cap);
        memset(d + used_cap, 0, new_cap - used_cap);
    }
    usedX = x;
    usedCompX = c;
    usedY = y;
    usedCompY = d;
    used_cap = new_cap;
}

/*  Grow the X or Y storage array as needed.  */
static void grow_seq_array(seq_t **arr, size_t *cap)
{
    size_t new_cap = *cap ? (*cap << 1) : 4;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    *arr = tmp;
    *cap = new_cap;
}

/*  Pointers to the next candidate values for X and Y.  */
static idx_t candX = 1;
static idx_t candY = 1;

/*  Generate terms up to index n (1‑based).  */
static void ensure_terms(idx_t n)
{
    if (n <= X_len) /* already computed */ return;

    while (X_len < n) {
        /* Find next X(n).  */
        while (candX < used_cap && (usedX[candX] || usedCompX[candX]))
            ++candX;
        /* Make sure the flag arrays can index candX. */
        grow_used_arrays((size_t)candX);

        seq_t x_val = (seq_t)candX;
        usedX[candX] = 1;                /* mark as belonging to X */

        /* Find next Y(n).  */
        while (candY < used_cap && (usedY[candY] || usedCompY[candY]))
            ++candY;
        grow_used_arrays((size_t)candY);

        seq_t y_val = (seq_t)candY;
        usedY[candY] = 1;                /* mark as belonging to Y */

        /* Mark the complement numbers.  */
        seq_t compX_val = (seq_t)( (seq_t)2 * x_val + y_val );
        seq_t compY_val = (seq_t)( x_val + y_val );

        /* Ensure flag arrays cover the complement indices.  */
        grow_used_arrays((size_t)compX_val);
        grow_used_arrays((size_t)compY_val);

        usedCompX[(size_t)compX_val] = 1;
        usedCompY[(size_t)compY_val] = 1;

        /* Store the current terms.  */
        if (X_len >= X_cap)
            grow_seq_array(&X_seq, &X_cap);
        X_seq[X_len] = x_val;
        ++X_len;

        if (Y_len >= Y_cap)
            grow_seq_array(&Y_seq, &Y_cap);
        Y_seq[Y_len] = y_val;
        ++Y_len;

        ++candX;  /* move to next candidate for future iterations */
        ++candY;
    }
}

/*  Public function requested by the build system.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n=0; safe fallback */
    ensure_terms(n);
    return Y_seq[n - 1];   /* 1‑based indexing */
}
