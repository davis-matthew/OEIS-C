/*
 * A018638  Divisors of 774.
 * https://oeis.org/A018638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 774 in ascending order
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* static cache of the 12 divisors of 774, in ascending order */
static seq_t divisors[12];
static idx_t div_count = 0;

/* Build the divisor list only once */
static void compute_divisors(void)
{
    if (div_count) return;          /* already built */

    const idx_t N  = 774ULL;        /* number whose divisors we need */
    idx_t limit = 1;
    while (limit * limit <= N) limit++;  /* find floor(sqrt(N)) */
    limit--;                         /* correct for the last increment */

    /* Temporary arrays to hold the small and large divisors */
    idx_t small[12];
    idx_t large[12];
    idx_t small_c = 0;
    idx_t large_c = 0;

    for (idx_t i = 1; i <= limit; ++i)
    {
        if (N % i == 0)             /* i is a divisor */
        {
            small[small_c++] = i;   /* store the small divisor */
            if (i != N / i)         /* avoid duplicate if perfect square */
                large[large_c++] = N / i;   /* store the large divisor */
        }
    }

    /* Copy small divisors (already ascending) */
    for (idx_t k = 0; k < small_c; ++k)
        divisors[k] = (seq_t) small[k];

    /* Append the large divisors in reverse order to keep ascending order */
    for (idx_t k = 0; k < large_c; ++k)
        divisors[small_c + k] = (seq_t) large[large_c - k - 1];

    div_count = small_c + large_c;   /* should be 12 for 774 */
}

/* Returns the n-th divisor of 774 (1‑based) or 0 if n is out of range. */
seq_t a(idx_t n)
{
    compute_divisors();

    if (n == 0 || n > div_count)    /* out of range; sequence is finite */
        return 0;

    return divisors[n - 1];
}
