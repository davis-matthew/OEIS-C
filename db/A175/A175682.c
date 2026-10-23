/*
 * A175682  Maximal run length of positive primes of the form ..., n-2*3*5,
 *          n-2*3, n-2, n, n+2, n+2*3, n+2*3*5, ...
 * https://oeis.org/A175682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 40, u32 n <= 40, u64 n <= 9821,
 *            u128 n <= 9821, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Maximal run length of positive primes of the form
 * n − 2 ∏p, n − 2 ∏p, n, and n + 2 ∏p where the product runs over
 * consecutive primes ≥ 3 in order: 2, 3, 5, 7, 11,….
 * a(n) = 1 + (# of such numbers < n that are prime) + (# of such numbers > n that are prime).
 * The sequence is computed by generating the sequence of shifts
 * 2*D_k (with D_0 = 1 and D_{k+1} = D_k * next_prime≥3)
 * up to the point where the shift would overflow the integer type
 * used for seq_t.  Then for each n the primes to the left and right
 * of n are counted until a composite or non‑positive value is found.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Primes ≥ 3, few are enough for n ≤ 10 000 but the array is safe for
 * much larger n when seq_t is an arbitrary‑precision type. */
static const uint64_t primes_small[] = {
    3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,
    37ULL,41ULL,43ULL,47ULL,53ULL,59ULL,61ULL,67ULL,71ULL,73ULL,
    79ULL,83ULL,89ULL,97ULL,101ULL,103ULL,107ULL,109ULL,113ULL
};
#define PRIMES_COUNT (sizeof(primes_small)/sizeof(primes_small[0]))
#define D_ARRAY_SIZE (PRIMES_COUNT+1)

/* Pre‑computed products  D_k  (D_0 = 1).  D has at least one element. */
static seq_t D[D_ARRAY_SIZE];
static idx_t max_k = 0;          /* last usable index in D */
static bool d_initialized = false;

/* Initialise D; done only once. */
static void init_D(void)
{
    if (d_initialized) return;
    D[0] = (seq_t)1;
    max_k = 0;
    for (idx_t i = 0; i < PRIMES_COUNT; ++i)
    {
        seq_t prod = D[i] * (seq_t)primes_small[i];
        /* stop when the product would overflow a uint64_t.
         * For arbitrary‑precision seq_t this comparison is harmless,
         * because |prod| will still be representable as seq_t. */
        if (prod > (seq_t)UINT64_MAX) break;
        D[i+1] = prod;
        max_k = i+1;
    }
    d_initialized = true;
}

/* a(idx_t n) – maximal run length of primes of the described form. */
seq_t a(idx_t n)
{
    init_D();
    seq_t N = (seq_t)n;
    /* If n itself is not prime, the run length is zero. */
    if (!is_prime_big(N)) return (seq_t)0;

    /* Count primes on the left side, including n. */
    seq_t left_count = (seq_t)1;
    for (idx_t k = 0; k <= max_k; ++k)
    {
        seq_t shift = (seq_t)2 * D[k];
        if (N <= shift) break;           /* would produce a non‑positive value */
        seq_t val = N - shift;
        if (!is_prime_big(val)) break;
        ++left_count;
    }

    /* Count primes on the right side, excluding n. */
    seq_t right_count = (seq_t)0;
    for (idx_t k = 0; k <= max_k; ++k)
    {
        seq_t shift = (seq_t)2 * D[k];
        seq_t val = N + shift;
        if (!is_prime_big(val)) break;
        ++right_count;
    }

    return left_count + right_count;
}
