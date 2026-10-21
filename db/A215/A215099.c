/*
 * A215099  a(0)=0, a(1)=1, a(n) = least k>a(n-1) such that k+a(n-2) is prime.
 * https://oeis.org/A215099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 3051, u32 n <= 3052, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 0,
   a(1) = 1,
   a(n) = least k > a(n-1) such that k + a(n-2) is prime
   (search starts at k = a(n-1) + 1). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* cached sequence */
static seq_t *seq = NULL;      /* array of computed values */
static size_t seq_len = 0;     /* number of values stored (0 for none yet) */
static size_t seq_cap = 0;     /* allocated capacity of seq */

/* Ensure that seq has capacity for indices up to 'needed' (inclusive). */
static void ensure_capacity(idx_t needed)
{
    /* allocate at least needed+1 slots */
    if (seq_cap > (size_t)needed) return;   /* enough already */
    size_t new_cap = (size_t)needed + 1;
    seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = new_seq;
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* fast return for already computed values */
    if ((size_t)n < seq_len) return seq[n];

    /* make sure we can store up to index n */
    ensure_capacity(n);

    /* compute missing terms from seq_len to n inclusive */
    for (idx_t i = seq_len; i <= n; ++i)
    {
        if (i == 0) {
            seq[0] = 0;    /* base case */
        } else if (i == 1) {
            seq[1] = 1;    /* base case */
        } else {
            seq_t a_nm1 = seq[i-1];
            seq_t a_nm2 = seq[i-2];
            /* start searching from the smallest integer > a(i-1) */
            seq_t k = a_nm1 + 1;
            seq_t sum = k + a_nm2;

            /* find smallest k with k + a(i-2) prime */
            while (!is_prime_big(sum))
            {
                k++;
                sum++;   /* k + a_nm2 incremented by 1 each step */
            }
            seq[i] = k;
        }
        seq_len = (size_t)i + 1;   /* one more term stored */
    }
    return seq[(size_t)n];
}
