/*
 * A176629  a(n) = n-th semiprime - (-1)^(n-th semiprime).
 * https://oeis.org/A176629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A001358(n) - (-1)^A001358(n)
 *            (n‑th semiprime minus (-1) to the power of that semiprime)
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for primes and semiprimes */
static uint64_t *primes = NULL;       /* primes up to current_limit     */
static size_t   primes_len = 0;       /* number of primes stored        */
static seq_t    *semis  = NULL;       /* semiprimes in ascending order  */
static size_t   semis_len = 0;        /* number of semiprimes stored    */
static idx_t    current_limit = 0;    /* maximum value sieved so far    */

/* Allocate a fresh array that can contain `cap` seq_t elements and
   placement‑construct each element.  This works for both POD and
   non‑POD seq_t types in C++ and trivially sized types in C. */
static seq_t *alloc_seq_arr(size_t cap)
{
    seq_t *arr = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!arr) return NULL;
    /* Construct each element if seq_t is a C++ class.  The cast is
       harmless for POD types. */
#ifdef __cplusplus
    for (size_t i = 0; i < cap; ++i)
        ::new (arr + i) seq_t();
#endif
    return arr;
}

/* Build primes up to 'limit' and generate all semiprimes <= limit.  The
   existing global data (primes, semis) are freed and replaced. */
static void build_primes_and_semis(idx_t limit)
{
    /* 1. Sieve to find primes up to `limit`. */
    bool *is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_prime) return;
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = true;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* 2. Collect primes into an array. */
    size_t pc = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++pc;
    uint64_t *p_arr = (uint64_t *)malloc(pc * sizeof(uint64_t));
    if (!p_arr) { free(is_prime); return; }
    size_t p_idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) p_arr[p_idx++] = (uint64_t)i;

    /* 3. Generate all semiprimes <= limit. */
    size_t s_cap = 8;
    seq_t *s_arr = alloc_seq_arr(s_cap);
    size_t s_cnt = 0;
    for (size_t a = 0; a < pc; ++a) {
        uint64_t pa = p_arr[a];
        for (size_t b = a; b < pc; ++b) {
            uint64_t pb = p_arr[b];
            idx_t prod = (idx_t)pa * (idx_t)pb;
            if (prod > limit) break;
            /* resize if necessary */
            if (s_cnt == s_cap) {
                s_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(s_arr, s_cap * sizeof(seq_t));
                if (!tmp) { free(is_prime); free(p_arr); free(s_arr); return; }
                s_arr = tmp;
#ifdef __cplusplus
                /* Construct the newly allocated slots. */
                for (size_t i = s_cnt; i < s_cap; ++i)
                    ::new (s_arr + i) seq_t();
#endif
            }
            s_arr[s_cnt++] = (seq_t)prod;
        }
    }

    /* 4. Sort the semiprimes (simple bubble sort for brevity; for 10k
       elements the cost is negligible).  For larger data a faster sort
       should be used (e.g., qsort). */
    for (size_t i = 0; i < s_cnt; ++i)
        for (size_t j = i + 1; j < s_cnt; ++j)
            if (s_arr[i] > s_arr[j]) {
                seq_t tmp = s_arr[i];
                s_arr[i] = s_arr[j];
                s_arr[j] = tmp;
            }

    /* 5. Release old data and store the new data. */
    free(is_prime);
    free(primes);
    free(semis);
    primes      = p_arr;
    primes_len  = pc;
    semis       = s_arr;
    semis_len   = s_cnt;
    current_limit = limit;
}

/* Ensure that at least `n` semiprimes are available. */
static void ensure_semiprimes(idx_t n)
{
    if (semis_len >= n) return;
    idx_t estimate = current_limit ? current_limit * 2 : (n > 10 ? n * 15 : 100);
    while (true) {
        build_primes_and_semis(estimate);
        if (semis_len >= n) return;
        estimate *= 2;
    }
}

/* Public function implementing A176629. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1: only n>=1 is meaningful */
    ensure_semiprimes(n);
    seq_t s = semis[n - 1];
    /* If s is even, (-1)^s = +1 → subtract 1; if odd, (-1)^s = -1 → add 1. */
    return (s & (seq_t)1) ? (s + 1) : (s - 1);
}
