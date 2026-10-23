/*
 * A035698  Number of partitions of n into parts 8k+5 and 8k+7 with at least
 *          one part of each type.
 * https://oeis.org/A035698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 209, u32 n <= 570, u64 n <= 1767,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A035698 – number of partitions of n into parts 8k+5 and 8k+7
 * with at least one part of each type.
 * Computation:  a(n) = P_all(n) – P_5(n) – P_7(n)
 * where P_all counts partitions using parts 5 or 7 mod 8,
 * P_5 counts partitions using only parts 5 mod 8,
 * P_7 counts partitions using only parts 7 mod 8.  */
/* NOTE: the function is allowed to use malloc/free in C mode and
 * new/delete[] in C++ mode to allocate arrays of seq_t.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper allocation functions that work for both C and C++. */
static seq_t *alloc_arr(idx_t size) {
#if defined(__cplusplus)
    return new seq_t[(size_t)size];
#else
    return (seq_t*)calloc((size_t)size, sizeof(seq_t));
#endif
}
static void free_arr(seq_t *p) {
#if defined(__cplusplus)
    delete [] p;
#else
    free(p);
#endif
}

/* Static state for memoisation. */
static bool initialized = false;
static idx_t max_n = 0;
static seq_t *T_arr = NULL;    /* partitions using parts 5 or 7 mod 8   */
static seq_t *A_arr = NULL;    /* partitions using only 5 mod 8        */
static seq_t *B_arr = NULL;    /* partitions using only 7 mod 8        */
static seq_t *Ans_arr = NULL;  /* final sequence values              */

/* Recompute all tables up to new_max. */
static void recompute(idx_t new_max) {
    idx_t sz = new_max + 1;

    seq_t *newT   = alloc_arr(sz);
    seq_t *newA   = alloc_arr(sz);
    seq_t *newB   = alloc_arr(sz);
    seq_t *newAns = alloc_arr(sz);

    /* Initialise all entries to zero.  For C++ builtins the values are
     * indeterminate; for built‑in integer types we must set them explicitly.
     */
    for (idx_t i = 0; i < sz; ++i) {
        newT[i]   = 0;
        newA[i]   = 0;
        newB[i]   = 0;
        newAns[i] = 0;
    }

    /* Base case: empty partition count. */
    newT[0] = newA[0] = newB[0] = 1;

    /* DP for A: parts ≡ 5 (mod 8). */
    for (idx_t k = 0;; ++k) {
        idx_t p = 8 * k + 5;
        if (p > new_max) break;
        for (idx_t s = p; s <= new_max; ++s) {
            newA[s] += newA[s - p];
        }
    }

    /* DP for B: parts ≡ 7 (mod 8). */
    for (idx_t k = 0;; ++k) {
        idx_t p = 8 * k + 7;
        if (p > new_max) break;
        for (idx_t s = p; s <= new_max; ++s) {
            newB[s] += newB[s - p];
        }
    }

    /* DP for T: all allowed parts (5 or 7 mod 8). */
    for (idx_t k = 0;; ++k) {
        idx_t p = 8 * k + 5;
        if (p > new_max) break;
        for (idx_t s = p; s <= new_max; ++s) {
            newT[s] += newT[s - p];
        }
    }
    for (idx_t k = 0;; ++k) {
        idx_t p = 8 * k + 7;
        if (p > new_max) break;
        for (idx_t s = p; s <= new_max; ++s) {
            newT[s] += newT[s - p];
        }
    }

    /* Final result: at least one part of each type. */
    for (idx_t i = 1; i <= new_max; ++i) {
        newAns[i] = newT[i] - newA[i] - newB[i];
    }

    /* Free old arrays. */
    if (T_arr   != NULL) free_arr(T_arr);
    if (A_arr   != NULL) free_arr(A_arr);
    if (B_arr   != NULL) free_arr(B_arr);
    if (Ans_arr != NULL) free_arr(Ans_arr);

    /* Store new arrays. */
    T_arr   = newT;
    A_arr   = newA;
    B_arr   = newB;
    Ans_arr = newAns;

    max_n = new_max;
    initialized = true;
}

/* External linkage: required sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;              /* undefined for n=0 */
    if (!initialized || n > max_n) {
        recompute(n);
    }
    return Ans_arr[n];
}
