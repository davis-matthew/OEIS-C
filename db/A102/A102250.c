/*
 * A102250  Indices of semiprime Haüy rhombic dodecahedral numbers.
 * https://oeis.org/A102250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A102250 – Indices of semiprime Haüy rhombic dodecahedral numbers.
 *
 * This function returns the n‑th index k for which
 *    2*k-1  and  8*k^2-14*k+7
 * are both prime.  The first values are 2, 3, 4, 6, 12, …
 *
 * The implementation searches incrementally and stores the found
 * indices in a dynamically resized array.  The code works for all
 * builds (C11 or C++17) and for all allowable integer types.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

static idx_t *valid = NULL;      /* indices that satisfy the condition   */
static idx_t  count  = 0;        /* number of stored indices             */
static idx_t  capacity = 0;      /* current capacity of the array        */
static idx_t  next    = 1;       /* next candidate to test               */

/* enlarge the array to hold at least 'need' elements */
static void ensure_capacity(idx_t need)
{
    if (need < capacity)
        return;
    idx_t newcap = capacity ? capacity << 1 : 128;
    while (newcap <= need)
        newcap <<= 1;
    idx_t *tmp = (idx_t *)realloc(valid, newcap * sizeof(*tmp));
    if (!tmp)
        abort();                   /* out of memory */
    valid = tmp;
    capacity = newcap;
}

/* return the n-th term of the sequence (a(1) is the first term) */
extern seq_t a(idx_t n)
{
    if (n == 0)                    /* undefined for 0 – harmless */
        return (seq_t)0;

    /* Generate valid indices until we have at least n of them */
    while (count < n) {
        uint64_t u = (uint64_t)next;

        /* Compute the two factors of the Haüy number */
        uint64_t p1 = 2 * u - 1;                     /* 2*n-1    */
        uint64_t p2 = 8 * u * u - 14 * u + 7;       /* 8*n^2-14*n+7 */

        /* Check primeness of both factors */
        if (is_prime_uint64(p1) && is_prime_uint64(p2)) {
            ensure_capacity(count + 1);
            valid[count++] = next;
        }
        ++next;
    }

    /* Return the stored index cast to the generic sequence type */
    return (seq_t)valid[n - 1];
}
