/*
 * A219860  a(n) is the smallest number greater than a(n-1) such that
 *          sigma(a(1)) + sigma(a(2)) + ... + sigma(a(n)) is prime.
 * https://oeis.org/A219860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 305, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A219860:  a(n) = smallest integer > a(n‑1) such that
   Σ_{i=1..n} sigma(a(i)) is prime.
   Computation is by incremental search and memoisation.
   The algorithm keeps the cumulative sum of sigmas and generates terms
   on demand.  It stores previously computed terms in a dynamic array;
   the implementation is C/C++ portable: in C it uses malloc/realloc,
   in C++ it uses std::vector.
*/
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sigma_of.h"
#include "../utils/isPrimeGeneric.h"

#ifdef __cplusplus
#include <vector>
#endif

/* Global state ----------------------------------------------------------*/

static seq_t cur_sum   = 0;   /* Σ sigma(a(1)) … sigma(a(a_len))           */
static seq_t last_val  = 1;   /* a(a_len)  (initialised to 1 for search)   */
static size_t a_len    = 0;   /* number of terms already computed          */

#ifndef __cplusplus
/* C implementation: dynamic array of seq_t --------------------------------------------------*/
static seq_t *a_vals = NULL;
static size_t a_cap  = 0;

static void grow_a_array(size_t new_cap)
{
    if (new_cap <= a_cap) return;
    seq_t *new_arr = realloc(a_vals, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    a_vals = new_arr;
    a_cap  = new_cap;
}

static inline seq_t get_value(size_t idx)   { return a_vals[idx]; }
static inline void add_value(seq_t val)
{
    if (a_len == a_cap) grow_a_array(a_cap ? a_cap*2 : 16);
    a_vals[a_len++] = val;
}
#else
/* C++ implementation: std::vector --------------------------------------------------------*/
static std::vector<seq_t> a_vec;

static inline seq_t get_value(size_t idx)   { return a_vec[idx]; }
static inline void add_value(seq_t val)
{
    a_vec.push_back(val);
    a_len = a_vec.size();   /* keep a_len in sync */
}
#endif

/* Generate the next term of the sequence ---------------------------------*/
static void generate_next(void)
{
    for (idx_t cand = (idx_t)last_val + 1;; ++cand) {
        seq_t sigma   = sigma_of(cand);                /* sigma(cand)  */
        seq_t new_sum = cur_sum + sigma;              /* new cumulative sum */
        if (isPrimeGeneric(new_sum)) {
            seq_t val = (seq_t)cand;                  /* store as seq_t   */
            add_value(val);
            cur_sum  = new_sum;
            last_val = val;                          /* update last term */
            break;
        }
    }
}

/* Public API --------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; behaviour for 0 is irrelevant */
    size_t target = (size_t)n;
    while (a_len < target) generate_next();  /* build up to required index */
    return get_value(target - 1);            /* a(n) is at index n‑1                   */
}
