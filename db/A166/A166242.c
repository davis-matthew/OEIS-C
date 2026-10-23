/*
 * A166242  Sequence generated from A014577, the dragon curve.
 * https://oeis.org/A166242
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 168, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A166242
 * a(-1) = 1
 * a(n+1) = 2*a(n) if A014577(n+1) = 1,
 *          a(n)/2   otherwise
 *
 * The dragon curve sequence A014577 is generated recursively:
 *   start with a single 1,
 *   at each step append 1 and then the reverse complement of the
 *   current bit string.  The result is stored in `dragon` so that
 *   dragon[i] (i ≥ 1) equals A014577(i).
 *
 * This implementation memoises both the dragon curve bits and the
 * sequence values so that calls in arbitrary order remain fast.
 */

#include <stdlib.h>    /* malloc, realloc, abort */
#include <stddef.h>    /* size_t */

/* Dragon curve bits (1‑based indexing); index 0 unused. */
static unsigned char *dragon = NULL;
static size_t  dragon_len  = 0;   /* current filled length */

/* Ensure dragon bits are available up to at least index `needed`. */
static void _ensure_dragon(size_t needed)
{
    if (dragon_len >= needed)
        return;

    if (dragon_len == 0) {
        /* first bit (index 1) */
        dragon        = (unsigned char*)malloc((needed + 1) * sizeof(unsigned char));
        if (!dragon) abort();
        dragon[1] = 1;
        dragon_len = 1;
    }

    while (dragon_len < needed) {
        size_t old_len = dragon_len;
        size_t new_len = 2 * old_len + 1; /* length after one folding step */

        unsigned char *tmp = (unsigned char*)realloc(dragon,
                                    (new_len + 1) * sizeof(unsigned char));
        if (!tmp) abort();
        dragon = tmp;

        /* middle bit is always 1 */
        dragon[old_len + 1] = 1;

        /* reverse complement of the old string */
        for (size_t pos = 1; pos <= old_len; ++pos) {
            dragon[new_len - pos + 1] = dragon[pos] ^ 1;
        }

        dragon_len = new_len;
    }
}

/* Cached values of the sequence */
static seq_t  *seq  = NULL;   /* array of values */
static size_t seq_cap = 0;    /* allocated capacity */
static size_t seq_len = 0;    /* number of values stored */

/* Main function required by the framework */
seq_t a(idx_t n)
{
    if (n < -1)            /* behaviour below offset is irrelevant */
        return 0;

    /* target index in the 0‑based seq array */
    size_t target = (size_t)(n + 1);

    /* Initialise storage for a(-1) if this is the first call */
    if (!seq) {
        seq = (seq_t*)malloc(sizeof(seq_t));
        if (!seq) abort();
        seq[0] = 1;            /* a(-1) */
        seq_cap = 1;
        seq_len = 1;
    }

    /* Expand seq array capacity if needed */
    if (target >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap : 1;
        while (new_cap <= target) new_cap <<= 1;

        seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq = tmp;
        seq_cap = new_cap;
    }

    /* Ensure enough dragon bits for the required indices */
    _ensure_dragon(target + 1); /* we need dragon[1 .. target] */

    /* Compute missing terms using the recurrence */
    for (size_t k = seq_len; k <= target; ++k) {
        seq[k] = (dragon[k] == 1) ? seq[k - 1] * 2
                                  : seq[k - 1] / 2;
    }
    seq_len = target + 1;

    return seq[target];
}
