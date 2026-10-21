/*
 * A058979  Number of numbers m such that phi(m) = 4n.
 * https://oeis.org/A058979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to φ(m) = 4*n
 * Range:     u8 n <= 6479, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 249 = number of m up to a million with φ(m)<=1 million (used for size estimate) */
/* This implementation precomputes Euler's totient function up to a limit that is
   comfortably larger than 4*n for all calls, and counts occurrences.   */
/* It uses a simple linear sieve to compute phi efficiently in O(L log log L).   */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Utility: ensure this array is at least size 'needed'.  Recompute phi if
   necessary. */
static void ensure_phi_and_counts(idx_t max_n);

/* Storage */
static unsigned *phi_arr = NULL;      /* phi values, phi_arr[i] = φ(i)   */
static size_t phi_cap = 0;            /* allocated size of phi array      */
static size_t count_cap = 0;          /* allocated size of count array    */
static seq_t *count_arr = NULL;       /* count_arr[k] = number of m with φ(m)=k  */
static idx_t counted_upto_n = 0;      /* maximum n for which counts are ready  */

/* Helper: linear sieve to compute phi up to limit L (exclusive) */
static void compute_phi(size_t L)
{
    if (L <= phi_cap) return; /* already enough */

    /* Resize and initialize phi array up to L */
    phi_arr = (unsigned *)realloc(phi_arr, L * sizeof(unsigned));
    if (!phi_arr) exit(1);
    for (size_t i = 0; i < L; ++i) phi_arr[i] = (unsigned)i;

    /* Sieve */
    for (size_t p = 2; p < L; ++p) {
        if (phi_arr[p] == (unsigned)p) {          /* prime */
            for (size_t k = p; k < L; k += p) {
                phi_arr[k] -= phi_arr[k] / (unsigned)p;
            }
        }
    }

    phi_cap = L;
}

/* Helper: recompute counts for all k up to target_k (using current phi_arr) */
static void recompute_counts(size_t target_k)
{
    if (target_k + 1 <= count_cap) return; /* already computed */

    /* Allocate or grow count array */
    count_arr = (seq_t *)realloc(count_arr, (target_k + 1) * sizeof(seq_t));
    if (!count_arr) exit(1);
    memset(count_arr, 0, (target_k + 1) * sizeof(seq_t));
    count_cap = target_k + 1;

    /* Count phi values */
    for (size_t i = 1; i < phi_cap; ++i) {
        unsigned k = phi_arr[i];
        if ((size_t)k <= target_k)
            count_arr[k] += 1;
    }
}

/* Main function called by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1 */

    /* Determine needed maximum k = 4*n */
    size_t target_k = 4 * (size_t)n;

    /* Ensure phi array covers enough m to capture all m with φ(m)<=target_k.
       Empirically, φ(m) ≥ m / log log m for large m, so m < 5*target_k
       suffices for target_k up to at least 1e6.  For safety, we use 5x. */
    size_t phi_limit = 5 * target_k;
    compute_phi(phi_limit);

    /* Recompute counts up to target_k if needed */
    recompute_counts(target_k);

    /* Return the count for φ(m)=4*n */
    return count_arr[target_k];
}
