/*
 * A061201  Partial sums of A007425: (tau<=)_3(n).
 * https://oeis.org/A061201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} tau(k) * floor(n/k)
 * Range:     u8 n <= 29, u16 n <= 1899, u32 n <= 9174, u64 n <= 9174,
 *            u128 n <= 9174, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where tau(k) is the divisor-count function A000005(k). */
/* This implementation uses a memoised divisor table and a
   grouping optimisation for the floor division. */

#ifdef __cplusplus
#include <vector>
#if !defined(_STDLIB_H) && !defined(_SIZE_T_DEFINED)
#  include <cstddef>
#endif
#else
#include <stdlib.h>
#include <stddef.h>
#endif

/* Forward declaration of seq_t and idx_t provided by the build system. */
typedef idx_t idx_t;
typedef seq_t seq_t;

/* Static storage for the divisor counts and their prefix sums. */
#if defined(__cplusplus)
/* C++ builds use a vector, which handles construction and destruction. */
static std::vector<seq_t> tau_vec;
static std::vector<seq_t> pref_vec;
static size_t pmax = 0;           /* maximum n currently stored */
#else
/* C builds use plain malloc-ed arrays. */
static seq_t *tau_arr = NULL;
static seq_t *pref_arr = NULL;
static size_t pmax = 0;           /* maximum n currently stored */
#endif

/* Rebuild the tables up to 'limit' (inclusive).  The previous tables
   are freed if present, and a new larger table is created.  If 'limit'
   is not greater than the current maximum, nothing is done. */
static void recompute(idx_t limit)
{
    if ((size_t)limit <= pmax)
        return;                    /* nothing to do */

#if defined(__cplusplus)
    /* Resize vectors to accommodate all indices up to 'limit'. */
    size_t sz = (size_t)limit + 1;
    tau_vec.assign(sz, (seq_t)0);
    pref_vec.assign(sz, (seq_t)0);

    /* Count divisors. */
    for (size_t i = 1; i <= limit; ++i) {
        for (size_t j = i; j <= limit; j += i)
            ++tau_vec[j];
    }

    /* Prefix sums of divisor counts. */
    for (size_t i = 1; i <= limit; ++i)
        pref_vec[i] = pref_vec[i - 1] + tau_vec[i];

    pmax = limit;
#else
    /* Allocate new arrays.  Use malloc and zero the memory.  We
       use a loop for zeroing to avoid relying on memset for non-POD
       types.  If malloc fails, exit, as the routine is called only
       internally. */
    seq_t *new_tau = (seq_t*)malloc(sizeof(seq_t) * (size_t)(limit + 1));
    seq_t *new_pref = (seq_t*)malloc(sizeof(seq_t) * (size_t)(limit + 1));
    if (!new_tau || !new_pref) {
        free(new_tau);
        free(new_pref);
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i <= limit; ++i) {
        new_tau[i] = (seq_t)0;
        new_pref[i] = (seq_t)0;
    }

    /* Count divisors. */
    for (size_t i = 1; i <= limit; ++i) {
        for (size_t j = i; j <= limit; j += i)
            ++new_tau[j];
    }

    /* Prefix sums of divisor counts. */
    for (size_t i = 1; i <= limit; ++i)
        new_pref[i] = new_pref[i - 1] + new_tau[i];

    /* Free old arrays. */
    if (tau_arr)  free(tau_arr);
    if (pref_arr) free(pref_arr);

    tau_arr = new_tau;
    pref_arr = new_pref;
    pmax = limit;
#endif
}

/* Public function computing the sequence value at position n. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* Offset 1; undefined for n=0 but safe */

    /* Ensure divisor tables cover up to n. */
    recompute(n);

    seq_t result = (seq_t)0;

#if defined(__cplusplus)
    /* Use the precomputed prefix sums. */
    for (size_t l = 1; l <= (size_t)n; ) {
        size_t q = (size_t)n / l;
        size_t r = (size_t)n / q;
        seq_t sum_tau = pref_vec[r] - pref_vec[l - 1];
        result += sum_tau * (seq_t)q;
        l = r + 1;
    }
#else
    for (size_t l = 1; l <= (size_t)n; ) {
        size_t q = (size_t)n / l;
        size_t r = (size_t)n / q;
        seq_t sum_tau = pref_arr[r] - pref_arr[l - 1];
        result += sum_tau * (seq_t)q;
        l = r + 1;
    }
#endif

    return result;
}
