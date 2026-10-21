/*
 * A054598  a(0)=0; for n>0, a(n) = Sum_{d|n} d*2^(n/d).
 * https://oeis.org/A054598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d*2^(n/d) (OEIS A054598)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* global array of powers of two: pow2[i] = 2^i  */
static seq_t *pow2 = NULL;
static idx_t pow2_size = 0;   /* number of entries currently valid [0 .. pow2_size-1] */
static idx_t pow2_cap   = 0;   /* allocated capacity */

/* ensure that pow2[n] exists for all 0 <= i <= n */
static void ensure_pow2(idx_t n)
{
    if (n < pow2_size) return;          /* already computed */

    /* choose a new capacity, at least n+1, and a power of two */
    size_t new_cap = pow2_cap;
    if (new_cap == 0) new_cap = 1;
    while (new_cap <= n) new_cap <<= 1;

    /* reallocate if necessary */
    if (pow2 == NULL) {
        pow2 = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!pow2) exit(1);
    } else if (new_cap > pow2_cap) {
        seq_t *tmp = (seq_t *)realloc(pow2, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        pow2 = tmp;
    }

    /* initialise values that were not yet computed */
    if (pow2_size == 0) {
        pow2[0] = (seq_t)1;           /* 2^0 */
        pow2_size = 1;
    }
    for (idx_t i = pow2_size; i <= n; ++i) {
        pow2[i] = pow2[i - 1] * (seq_t)2;
    }
    pow2_size = n + 1;
    pow2_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    ensure_pow2(n);

    seq_t sum = 0;
    /* iterate over all divisors up to sqrt(n) */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t term1 = (seq_t)d1 * pow2[n / d1];
            sum += term1;

            if (d1 != d2) {
                seq_t term2 = (seq_t)d2 * pow2[n / d2];
                sum += term2;
            }
        }
    }
    return sum;
}
