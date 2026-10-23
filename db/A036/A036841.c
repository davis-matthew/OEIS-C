/*
 * A036841  Smallest number k such that floor[(the number of digits in k!) /
 *          k] is greater than any j < k.
 * https://oeis.org/A036841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A036841
    Smallest number k such that floor[(#digits in k!)/k] is greater
    than any value obtained from smaller k.
    The function searches for record values of floor(digits(k!)/k)
    and stores the corresponding k’s in a cache.
*/
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static seq_t *seq_cache = NULL;   /* cache of sequence terms   */
static size_t seq_cap   = 0;      /* current capacity         */
static size_t seq_len   = 0;      /* number of terms cached   */

/* Ensure the dynamic array has room for at least 'needed' elements.  */
static void grow_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t newcap = (*cap) ? (*cap) * 2 : 16;
    while (newcap < needed) newcap <<= 1;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

/* Global state for the progressive scan of k.                     */
static idx_t   curr_k   = 1;          /* last k processed (1 at start) */
static long double log_sum = 0.0L;    /* sum of log10(i) for i=1..k   */
static idx_t   best_val = 0;          /* highest floor(digits(i!)/i) seen */

/* Return the nth term of the sequence (offset 1).                 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, safety */

    /* Build cache up to n terms. */
    while (seq_len < (size_t)n) {
        /* First call – set up the first term a(1)=2. */
        if (seq_len == 0) {
            grow_seq(&seq_cache, &seq_cap, 1);
            seq_cache[0] = (seq_t)2;
            seq_len = 1;
            curr_k   = 1;
            log_sum  = 0.0L;
            best_val = 0;
            continue;
        }

        ++curr_k;                          /* next k       */
        log_sum += log10l((long double)curr_k);

        /* digits(k!) = floor(log10(k!)) + 1 */
        long long digits = (long long)floorl(log_sum + 1e-12L) + 1L;
        idx_t val = (idx_t)(digits / curr_k);    /* floor division */

        if (val > best_val) {              /* new record   */
            grow_seq(&seq_cache, &seq_cap, seq_len + 1);
            seq_cache[seq_len++] = (seq_t)curr_k;
            best_val = val;
        }
    }
    return seq_cache[(size_t)(n - 1)];
}
