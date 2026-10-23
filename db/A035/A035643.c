/*
 * A035643  Number of partitions of n into parts 6k+1 and 6k+3 with at least
 *          one part of each type.
 * https://oeis.org/A035643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 106, u32 n <= 333, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A035643: Number of partitions of n into parts 6k+1 and 6k+3
 * with at least one part of each type.
 * Computed by dynamic programming via inclusion–exclusion:
 *   a(n) = P_all(n) - P1(n) - P3(n)
 * where P_all counts all partitions using parts of both types,
 * P1 counts partitions using only parts 6k+1,
 * and P3 counts partitions using only parts 6k+3. */

#include <stdlib.h>
#include <stddef.h>
#ifdef __cplusplus
#include <new>
#endif

static idx_t _max_n = 0;
static seq_t *P_all = NULL;
static seq_t *P1   = NULL;
static seq_t *P3   = NULL;

/* Rebuild all partition tables up to new_max. */
static void _rebuild(idx_t new_max)
{
    /* Allocate raw memory for the tables. */
    size_t sz = sizeof(seq_t) * (new_max + 1);
    void *mem_all = malloc(sz);
    void *mem_p1   = malloc(sz);
    void *mem_p3   = malloc(sz);
    if (!mem_all || !mem_p1 || !mem_p3)
        return; /* enough memory assumed */

    /* Free any previous tables. */
    if (P_all) free(P_all);
    if (P1)   free(P1);
    if (P3)   free(P3);

    P_all = (seq_t *)mem_all;
    P1   = (seq_t *)mem_p1;
    P3   = (seq_t *)mem_p3;

    /* Construct (or initialize) table entries. */
    for (idx_t i = 0; i <= new_max; ++i) {
#ifdef __cplusplus
        new(&P_all[i]) seq_t();
        new(&P1[i])   seq_t();
        new(&P3[i])   seq_t();
#else
        P_all[i] = 0;
        P1[i]   = 0;
        P3[i]   = 0;
#endif
    }
    /* Base case: one way to make sum 0. */
#ifdef __cplusplus
    P_all[0] = seq_t(1);
    P1[0]   = seq_t(1);
    P3[0]   = seq_t(1);
#else
    P_all[0] = 1;
    P1[0]   = 1;
    P3[0]   = 1;
#endif

    /* Add parts of the form 6k+1. */
    for (idx_t k = 0; ; ++k) {
        idx_t part = 6 * k + 1;
        if (part > new_max) break;
        for (idx_t j = part; j <= new_max; ++j) {
            P_all[j] += P_all[j - part];
            P1[j]   += P1[j - part];
        }
    }

    /* Add parts of the form 6k+3. */
    for (idx_t k = 0; ; ++k) {
        idx_t part = 6 * k + 3;
        if (part > new_max) break;
        for (idx_t j = part; j <= new_max; ++j) {
            P_all[j] += P_all[j - part];
            P3[j]   += P3[j - part];
        }
    }

    _max_n = new_max;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Safety: index 0 is outside the defined offset. */
#ifdef __cplusplus
        return seq_t(0);
#else
        return 0;
#endif
    }
    if (n > _max_n) {
        _rebuild(n);
    }
    seq_t result = P_all[n];
    result -= P1[n];
    result -= P3[n];
    return result;
}
