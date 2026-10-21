/*
 * A220336  A modified Engel expansion for 4*sqrt(2) - 5.
 * https://oeis.org/A220336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3n+1)=2; a(3n+2)=2*(a(3n-1)^2-2*a(3n-1)+1);
 *            a(3n+3)=2*a(3n+2)-2; Base values: a(1)=2, a(2)=4. (All other
 *            a(3n+1)=2.)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 13, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial cases directly */
    if (n == 1) return 2;
    if (n == 2) return 4;

    /* Determine the congruence class of n modulo 3 */
    unsigned mod = (unsigned)(n % 3U);

    /* For terms congruent to 1 mod 3, the value is always 2 */
    if (mod == 1) {
        return 2;
    }

    /* Compute k for the recurrence blocks */
    idx_t k;
    if (mod == 2)          /* n = 3k+2 */
        k = (n - 2) / 3;
    else                   /* mod == 0, n = 3k+3 */
        k = (n - 3) / 3;

    /* Global cache for the sequence b_k = a(3k+2) */
    static seq_t *B = NULL;
    static size_t B_len   = 0;   /* number of elements currently stored */
    static size_t B_cap   = 0;   /* allocated capacity */

    /* Ensure the cache can hold up to index k */
    size_t needed = (size_t)k + 1;
    if (B_cap < needed) {
        size_t new_cap = B_cap ? B_cap << 1 : 4;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(B, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        B = tmp;
        B_cap = new_cap;
    }

    /* Initialise the first element if not yet done */
    if (B_len == 0) {
        B[0] = 4;   /* a(2) */
        B_len = 1;
    }

    /* Fill the cache up to index k */
    while (B_len <= k) {
        seq_t prev = B[B_len - 1];
        seq_t tmp  = prev * prev;           /* prev^2 */
        tmp = tmp - 2 * prev + 1;           /* prev^2 - 2*prev + 1 */
        B[B_len] = 2 * tmp;                 /* 2*(prev^2 - 2*prev + 1) */
        B_len++;
    }

    /* Return the requested value */
    if (mod == 2) {            /* a(3k+2) itself */
        return B[(size_t)k];
    } else {                   /* mod == 0 -> a(3k+3) = 2*b_k - 2 */
        return 2 * B[(size_t)k] - 2;
    }
}
