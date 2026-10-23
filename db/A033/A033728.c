/*
 * A033728  Product theta3(q^d); d | 16.
 * https://oeis.org/A033728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in the product of theta_3(q^d) for
 *            d|16
 * Range:     u8 n <= 69, u16 n <= 2607, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the generating function by successive convolution
   of the series of theta_3(q^d) for d = 1,2,4,8,16.  It stores the result in
   a static cache so that subsequent calls are O(1). */

#include <stdlib.h>

/* Utility allocation / deallocation functions that work both in C and C++.
   The functions zero‑initialize the array elements which is required for
   arbitrary‑precision integer types where the default constructor sets the
   value to zero. */
static seq_t *alloc_seq_array(idx_t size)
{
#ifdef __cplusplus
    /* Zero‑initialise every element */
    return new seq_t[size]();
#else
    /* C: calloc zero‑initialises */
    return (seq_t *)calloc((size_t)size, sizeof(seq_t));
#endif
}

static void free_seq_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Divisors of 16 that appear in the product */
static const idx_t divisors[] = {1, 2, 4, 8, 16};
static const size_t num_divisors = 5;

/* Cached sequence values (0 .. built_upto‑1) */
static seq_t *sequence = NULL;
static idx_t built_upto = 0;        /* number of terms currently stored */

/* Build the sequence up to and including 'limit' (0‑based). */
static void build_sequence(idx_t limit)
{
    /* Allocate space for all required terms */
    idx_t size = limit + 1;                /* indices 0 … limit inclusive */
    seq_t *cur = alloc_seq_array(size);
    if (!cur) return;                      /* allocation failure – silently ignore */

    cur[0] = (seq_t)1;                     /* theta_3(1) coefficient at q^0 */

    /* Convolve with each theta_3(q^d) factor in turn. */
    for (size_t di = 0; di < num_divisors; ++di)
    {
        idx_t d = divisors[di];
        seq_t *next = alloc_seq_array(size);
        if (!next) {                       /* allocation failure – clean up */
            free_seq_array(cur);
            return;
        }

        for (idx_t i = 0; i <= limit; ++i)
        {
            seq_t val = cur[i];
            if (!val) continue;           /* skip zero terms */

            /* m = 0 contributes coefficient 1 */
            next[i] += val;

            /* m > 0 contributes coefficient 2 for each square multiple of d */
            for (idx_t m = 1; ; ++m)
            {
                idx_t w = d * m * m;        /* weight of q^(d*m^2) */
                if (w > limit) break;
                idx_t j = i + w;
                if (j > limit) break;
                next[j] += val * (seq_t)2;
            }
        }

        free_seq_array(cur);
        cur = next;
    }

    /* Replace the old cache with the newly computed sequence. */
    free_seq_array(sequence);
    sequence   = cur;
    built_upto = size;
}

/* Public entry point: returns the nth term of the sequence.
   The function is idempotent and works for calls in any order. */
seq_t a(idx_t n)
{
    if (!sequence || n >= built_upto)
        build_sequence(n);
    return sequence[n];
}
