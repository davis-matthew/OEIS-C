/*
 * A214915  a(0)=a(1)=1, a(n) = least k > a(n-1) such that k*a(n-2) is a Lucas
 *          number.
 * https://oeis.org/A214915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence:  a(0)=a(1)=1,  a(n)=least k> a(n-1) such that  k*a(n-2)  is a Lucas number */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cache for previously computed values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure cache has space for indices up to n */
static void grow_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_cap >= needed) return;

    seq_t *new_arr = (seq_t*)malloc(needed * sizeof(seq_t));
    if (new_arr == NULL) abort();

    if (cache != NULL) {
        for (size_t i = 0; i < cache_len; ++i) new_arr[i] = cache[i];
        free(cache);
    }
    cache = new_arr;
    cache_cap = needed;
}

/* Initialise first two terms */
static void init_base(void)
{
    if (cache_len >= 2) return;
    grow_cache(1);               /* need indices 0 and 1 */

    cache[0] = 1;
    cache[1] = 1;
    cache_len = 2;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_base();

    if ((size_t)n < cache_len) return cache[(size_t)n];

    grow_cache(n);

    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t prev_luc = 2;        /* Lucas number L0 */
        seq_t curr_luc = 1;        /* Lucas number L1 */

        seq_t a_nm2 = cache[i-2];
        seq_t a_nm1 = cache[i-1];

        seq_t threshold = a_nm2 * a_nm1;   /* k must satisfy k > a_nm1, so L > a_nm1*a_nm2 */

        seq_t result_k = 0;

        while (true) {
            /* check prev_luc */
            if (prev_luc > threshold && (prev_luc % a_nm2) == 0) {
                result_k = prev_luc / a_nm2;
                break;
            }
            /* check curr_luc */
            if (curr_luc > threshold && (curr_luc % a_nm2) == 0) {
                result_k = curr_luc / a_nm2;
                break;
            }
            /* generate next Lucas number */
            seq_t next_luc = prev_luc + curr_luc;
            prev_luc = curr_luc;
            curr_luc = next_luc;
        }
        cache[i] = result_k;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
