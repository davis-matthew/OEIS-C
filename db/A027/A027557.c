/*
 * A027557  Number of 3-balanced strings of length n: let d(S)= #(1)'s in S -
 *          #(0)'s, then S is k-balanced if every substring T has -k<=d(T)<=k;
 *          here k=3.
 * https://oeis.org/A027557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=4, a(3)=8; a(n) = a(n-1) + 3*a(n-2) -
 *            2*a(n-3) - 2*a(n-4) (for n>=4)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *seq = NULL;          /* cached sequence values */
static idx_t  seq_size = 0;        /* number of elements already computed */
static idx_t  seq_capacity = 0;    /* allocated capacity */

/* Ensure the cache can hold values up to index n. */
static void ensure_capacity(idx_t n)
{
    if (n < seq_capacity)
        return;
    idx_t new_cap = seq_capacity ? seq_capacity << 1 : 8;
    while (new_cap <= n)
        new_cap <<= 1;

#ifdef __cplusplus
    /* C++: use new[] and delete[] which run constructors/destructors */
    seq_t *tmp = new seq_t[new_cap];
    for (idx_t i = 0; i < seq_size; ++i)
        tmp[i] = seq[i];
    delete[] seq;
#else
    /* C: use calloc/ free which just allocates raw memory */
    seq_t *tmp = (seq_t*)calloc(new_cap, sizeof(seq_t));
    memcpy(tmp, seq, seq_size * sizeof(seq_t));
    free(seq);
#endif
    seq = tmp;
    seq_capacity = new_cap;
}

/* Build the sequence up to index n. */
static void build_to(idx_t n)
{
    if (n < seq_size)    /* already computed enough */
        return;

    ensure_capacity(n);

    /* First accessed index: initialise the first four values only once. */
    if (seq_size == 0) {
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        seq[2] = (seq_t)4;
        seq[3] = (seq_t)8;
        seq_size = 4;
    }

    for (idx_t i = seq_size; i <= n; ++i) {
        if (i >= 4) {
            /* recurrence relation – all operands are of type seq_t. */
            seq[i] = seq[i-1] + (seq_t)3 * seq[i-2]
                     - (seq_t)2 * seq[i-3] - (seq_t)2 * seq[i-4];
        }
        /* for i<4 the initial values already exist; nothing to do */
    }

    /* seq_size never decreases; keep the maximum reached value. */
    if (n + 1 > seq_size)
        seq_size = n + 1;
}

seq_t a(idx_t n)
{
    build_to(n);
    return seq[n];
}
