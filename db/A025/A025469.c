/*
 * A025469  Number of partitions of n into 3 distinct positive cubes.
 * https://oeis.org/A025469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to n = x^3 + y^3 + z^3 with x>y>z>0
 * Range:     u8 n <= 9911, u16 n <= 9911, u32 n <= 9911, u64 n <= 9911,
 *            u128 n <= 9911, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the count for any n >= 0.
   It precomputes all sums of three distinct positive cubes up to the
   largest argument requested so far, storing the counts in a static array.
   For C++ builds it uses operator new/new[] to construct seq_t objects,
   for C builds it uses calloc to obtain zero‑initialised storage. */

#include <stdlib.h>

/* static storage for precomputation */
static seq_t *sum_counts = NULL;
static idx_t max_precomp = 0;
static int allocated_with_new = 0;         /* 1 if new[] was used, 0 if calloc */

/* free existing allocation (called when a larger precomputation is needed) */
static void free_sum_counts(void)
{
    if (sum_counts) {
#if defined(__cplusplus)
        if (allocated_with_new)
            delete[] sum_counts;
        else
#endif
            free(sum_counts);
        sum_counts = NULL;
        max_precomp = 0;
        allocated_with_new = 0;
    }
}

/* precompute counts for all sums up to limit */
static void precompute(idx_t limit)
{
    if (limit <= max_precomp)
        return;                     /* already done up to this limit */

    free_sum_counts();               /* release previous storage */

    /* allocate array to hold counts */
    size_t size = (size_t)(limit + 1);
#if defined(__cplusplus)
    sum_counts = new seq_t[size];
    allocated_with_new = 1;
#else
    sum_counts = (seq_t*)calloc(size, sizeof(seq_t));
    allocated_with_new = 0;
#endif
    if (!sum_counts)                /* allocation failed – all future calls will return 0 */
        return;

    /* find largest integer root such that root^3 <= limit */
    idx_t max_root = 0;
    while ((max_root + 1) * (max_root + 1) * (max_root + 1) <= limit)
        ++max_root;

    /* generate cubes */
    size_t cube_size = (size_t)(max_root + 1);
    seq_t *cube;
#if defined(__cplusplus)
    cube = new seq_t[cube_size];
    allocated_with_new = 1;                 /* reusing flag for cube array is fine */
#else
    cube = (seq_t*)calloc(cube_size, sizeof(seq_t));
    allocated_with_new = 0;
#endif
    if (!cube) {
        free_sum_counts();
        return;
    }
    cube[0] = 0;
    for (idx_t i = 1; i <= max_root; ++i)
        cube[i] = (seq_t)i * (seq_t)i * (seq_t)i;

    /* count all triples x>y>z>0 */
    if (max_root >= 3) {
        for (idx_t z = 1; z <= max_root - 2; ++z) {
            seq_t cz = cube[z];
            for (idx_t y = z + 1; y <= max_root - 1; ++y) {
                seq_t syz = cz + cube[y];
                if (syz > (seq_t)limit)
                    break;
                for (idx_t x = y + 1; x <= max_root; ++x) {
                    seq_t s = syz + cube[x];
                    if (s > (seq_t)limit)
                        break;
                    /* sum_counts is zero‑initialised, increment by one */
                    sum_counts[(idx_t)s] += (seq_t)1;
                }
            }
        }
    }

    /* clean up cube array */
#if defined(__cplusplus)
    if (allocated_with_new)
        delete[] cube;
    else
#endif
        free(cube);

    max_precomp = limit;
}

seq_t a(idx_t n)
{
    precompute(n);
    if (!sum_counts)
        return (seq_t)0;          /* allocation failed – return zero */
    return sum_counts[n];
}
