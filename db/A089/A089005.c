/*
 * A089005  Number of partitions of n-set with at least one even block.
 * https://oeis.org/A089005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A089005: number of partitions of an n‑set having at least one even block.
    The sequence satisfies a(n) = B_n - O_n, where B_n are Bell numbers
    and O_n are the number of partitions into odd‑sized blocks.  */

/* Include the required standard headers. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/*  Function prototype – the types seq_t and idx_t are supplied by the build.  */
seq_t a(idx_t n)
{
    /* Allocate static tables for Bell numbers B and for odd‑only partitions O.  */
    static seq_t *B        = NULL;   /* B[0] = 1, etc.  */
    static seq_t *O        = NULL;   /* O[0] = 1, etc.  */
    static size_t Bsize    = 0;      /* Current allocated size. */
    static size_t Osize    = 0;
    static idx_t  computed = 0;      /* Last index computed (>=0). */

    /* Offset 1: a(1) is the first term; handle n < 1 simply. */
    if (n < 1) return 0;

    /* Ensure we have storage for indices up to n. */
    if (Bsize < (size_t)(n + 1) || Osize < (size_t)(n + 1)) {
        size_t need  = (size_t)(n + 1);
        size_t alloc = (Bsize < need) ? need * 2 : Bsize;
        if (B == NULL) {
            B = (seq_t *)malloc(alloc * sizeof(seq_t));
            if (!B) return 0;
        } else {
            B = (seq_t *)realloc(B, alloc * sizeof(seq_t));
            if (!B) return 0;
        }
        if (O == NULL) {
            O = (seq_t *)malloc(alloc * sizeof(seq_t));
            if (!O) return 0;
        } else {
            O = (seq_t *)realloc(O, alloc * sizeof(seq_t));
            if (!O) return 0;
        }
        Bsize = alloc;
        Osize = alloc;
    }

    /* Initialise base case if this is the first call. */
    if (computed == 0) {
        B[0] = 1;   /* Bell(0) = 1. */
        O[0] = 1;   /* One partition of the empty set (all blocks are odd). */
    }

    /* Compute values up to index n (inclusive) if necessary. */
    for (idx_t i = computed; i < n; ++i) {          /* i runs 0..n-1, compute i+1 */
        /* --- Compute B[i+1] ------------------------------------------------ */
        seq_t sumB   = 0;
        seq_t binomB = 1;                            /* C(i,0) */

        for (idx_t k = 0; k <= i; ++k) {
            sumB += binomB * B[k];
            if (k < i) {
                binomB = binomB * (i - k) / (k + 1);
            }
        }
        B[i + 1] = sumB;

        /* --- Compute O[i+1] ------------------------------------------------ */
        seq_t sumO   = 0;
        seq_t binomO = 1;                            /* C(i,0) */

        for (idx_t j = 0; j <= i; ++j) {             /* j = k-1; k = j+1 */
            idx_t k = j + 1;
            if ((k & 1) == 1) {                      /* k odd => block of odd size */
                sumO += binomO * O[i + 1 - k];
            }
            if (j < i) {
                binomO = binomO * (i - j) / (j + 1);
            }
        }
        O[i + 1] = sumO;
    }

    computed = n;                 /* records that indices up to n are ready. */

    /* Desired term A089005. */
    return B[n] - O[n];
}
