/*
 * A154487  a(n+1)-+a(n)=prime, a(1)=0,a(2)=7.
 * https://oeis.org/A154487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small, seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154487
 * a(1) = 0, a(2) = 7
 * For n > 2: let d = the smallest prime such that
 *     is_prime_big(2*a(n-1) + d) == true
 * then a(n) = a(n-1) + d
 * The sequence guarantees that for every consecutive pair
 * (a(k), a(k+1)) both the sum and difference are primes.  */

#include <stddef.h>                 /* size_t */
#include "../utils/is_prime_small.h" /* is_prime_small(idx_t)  */
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t)    */
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

static seq_t *seq_cache = NULL;   /* array storing a(1), a(2), ...      */
static size_t seq_size = 0;       /* number of terms currently computed */
static size_t seq_cap  = 0;       /* allocated capacity of seq_cache   */

/* Grow seq_cache to at least 'need' elements */
static inline void ensure_capacity(size_t need)
{
    if (seq_cap >= need)
        return;

    size_t newcap = seq_cap ? seq_cap << 1 : 4;
    while (newcap < need)
        newcap <<= 1;

    /* Allocate new array of seq_t */
    seq_t *new_arr = seq_alloc_array(newcap);

    /* Copy existing values */
    for (size_t i = 0; i < seq_size; ++i)
        new_arr[i] = seq_cache[i];

    /* Free old array if it existed */
    if (seq_cache)
        seq_free_array(seq_cache);

    seq_cache = new_arr;
    seq_cap  = newcap;
}

/* Initialise the sequence with the first two terms */
static inline void init_sequence(void)
{
    if (seq_size > 0)          /* already initialised */
        return;

    ensure_capacity(2);
    seq_cache[0] = (seq_t)0;    /* a(1) */
    seq_cache[1] = (seq_t)7;    /* a(2) */
    seq_size    = 2;
}

seq_t a(idx_t n)
{
    if (n == 0)                  /* undefined; oddsays n >= 1 */
        return (seq_t)0;

    ensure_capacity(n);
    init_sequence();

    /* Generate missing terms up to n */
    while (seq_size < (size_t)n)
    {
        seq_t cur = seq_cache[seq_size - 1];

        idx_t d = 2;             /* candidate difference */
        while (1)
        {
            if (is_prime_small(d))            /* d must be prime      */
            {
                /* Check whether 2*cur + d is prime */
                seq_t sum = cur * (seq_t)2 + (seq_t)d;
                if (is_prime_big(sum))
                {
                    seq_t next = cur + (seq_t)d;
                    seq_cache[seq_size] = next;
                    ++seq_size;
                    break;
                }
            }
            ++d;                /* try next candidate */
        }
    }

    return seq_cache[n - 1];
}
