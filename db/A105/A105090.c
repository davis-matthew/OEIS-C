/*
 * A105090  Sum of the left diagonal in ordered 3 X 3 prime squares.
 * https://oeis.org/A105090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = p(9n-8) + p(9n-4) + p(9n)  (sum of the left diagonal
 * of the ordered 3×3 prime square whose rows are consecutive primes)
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static storage for the list of primes */
static size_t prime_count = 0;
static uint64_t *prime_arr = NULL;

/* build a list of at least 'needed' primes using a simple sieve */
static void build_primes_needed(size_t needed)
{
    if (prime_count >= needed) return;

    /* crude upper bound:  (20 * needed) + 100 is safe for all n <= 10^5 */
    size_t limit = needed * 20 + 100;
    bool *is_comp = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_comp) return;           /* out of memory – win‑ferbled */

    is_comp[0] = is_comp[1] = true;
    for (size_t i = 2; i * i <= limit; ++i)
        if (!is_comp[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;

    size_t count = 0;
    uint64_t *arr = (uint64_t*)malloc(needed * sizeof(uint64_t));
    if (!arr) { free(is_comp); return; }

    for (size_t i = 2; i <= limit && count < needed; ++i)
        if (!is_comp[i])
            arr[count++] = i;

    free(is_comp);
    free(prime_arr);
    prime_arr = arr;
    prime_count = count;   /* should equal 'needed' due to generous bound */
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* out of defined range */
    size_t needed = 9 * (size_t)n;
    build_primes_needed(needed);

    /* zero‑based indices of the required primes */
    size_t i1 = 9 * (size_t)n - 9;   /* 9n-8  → index 9n-9 */
    size_t i2 = 9 * (size_t)n - 5;   /* 9n-4  → index 9n-5 */
    size_t i3 = 9 * (size_t)n - 1;   /* 9n    → index 9n-1 */

    seq_t sum = (seq_t)prime_arr[i1]
              + (seq_t)prime_arr[i2]
              + (seq_t)prime_arr[i3];
    return sum;
}
