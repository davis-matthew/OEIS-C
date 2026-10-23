/*
 * A035651  Number of partitions of n into parts 7k and 7k+1 with at least one
 *          part of each type.
 * https://oeis.org/A035651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_7,1(n) – p_7(n) – p_1(n) where p_7,1 counts partitions
 *            of n using parts 7k or 7k+1, p_7 counts partitions using only
 *            multiples of 7, p_1 counts partitions using only parts ≡1 (mod
 *            7)
 * Range:     u8 n <= 56, u16 n <= 143, u32 n <= 428, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper to allocate an array of seq_t that works both in C and C++. */
#ifdef __cplusplus
static seq_t *allocate_array(idx_t N) { return new seq_t[N + 1]; }
static void deallocate_array(seq_t *p) { delete[] p; }
#else
static seq_t *allocate_array(idx_t N) { return (seq_t *)malloc((N + 1) * sizeof(seq_t)); }
static void deallocate_array(seq_t *p) { free(p); }
#endif

/* Static caches for dynamic programming. */
static seq_t *allowed  = NULL;  /* parts ≡0 or 1 (mod 7)  */
static seq_t *Aarr     = NULL;  /* only multiples of 7  */
static seq_t *Barr     = NULL;  /* only parts ≡1 (mod 7) */
static idx_t   max_n   = 0;     /* largest n whose tables exist */

/* Recompute all tables up to N inclusive. */
static void recompute(idx_t N)
{
    /* Allocate new tables. */
    seq_t *new_allowed = allocate_array(N);
    seq_t *new_Aarr     = allocate_array(N);
    seq_t *new_Barr     = allocate_array(N);

    /* Zero all entries. */
    for (idx_t i = 0; i <= N; ++i) {
        new_allowed[i] = (seq_t)0;
        new_Aarr[i]    = (seq_t)0;
        new_Barr[i]    = (seq_t)0;
    }

    /* Base cases: one way to partition 0. */
    new_allowed[0] = new_Aarr[0] = new_Barr[0] = (seq_t)1;

    /* Dynamic programming over all possible part sizes. */
    for (idx_t part = (idx_t)1; part <= N; ++part) {
        idx_t mod7 = part % (idx_t)7;

        /* Updates for allowed parts (≡0 or ≡1). */
        if (mod7 == (idx_t)0 || mod7 == (idx_t)1) {
            for (idx_t i = part; i <= N; ++i)
                new_allowed[i] += new_allowed[i - part];
        }

        /* Updates for multiples of 7. */
        if (mod7 == (idx_t)0) {
            for (idx_t i = part; i <= N; ++i)
                new_Aarr[i] += new_Aarr[i - part];
        }

        /* Updates for parts ≡1 (mod 7). */
        if (mod7 == (idx_t)1) {
            for (idx_t i = part; i <= N; ++i)
                new_Barr[i] += new_Barr[i - part];
        }
    }

    /* Free old tables. */
    deallocate_array(allowed);
    deallocate_array(Aarr);
    deallocate_array(Barr);

    /* Store new tables. */
    allowed = new_allowed;
    Aarr    = new_Aarr;
    Barr    = new_Barr;
    max_n   = N;
}

/* Public function as required by the test harness. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0; /* offset starts at 1; a(1) is defined */

    /* Recompute tables up to the requested index if needed. */
    if (n > max_n)
        recompute(n);

    /* Compute the desired value: partitions with at least one part of each type. */
    return allowed[n] - Aarr[n] - Barr[n];
}
