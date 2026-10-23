/*
 * A011731  A binary m-sequence: expansion of reciprocal of x^18 + x^7 + 1
 *          (mod 2, shifted by 17 initial 0's).
 * https://oeis.org/A011731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-7) XOR a(n-18)  for n >= 18
 * Initial values: a(0..16)=0, a(17)=1
 * Periodicity: a(n+262143) = a(n)  (full period of the LFSR)
 *
 * Implementation notes:
 *   * Uses a static cache that contains one full period.
 *   * The cache is initialized on the first call.
 *   * For any n, we compute n % PERIOD and return the precomputed bit.
 *   * All operations respect the allowed list of operators.
 */
#include <stdlib.h>

/* The bit-length of the LFSR. */
#define PERIOD 262143   /* 2^18 - 1 */

/* Static storage for one full period of the sequence. */
static seq_t *cache = NULL;

/* Initialise the cache with one full period.
 * The cache is allocated on the first call of seq_t a(idx_t). */
static void init_cache(void)
{
    if (cache != NULL)            /* already initialised */
        return;

    /* Allocate and zero-initialise the cache. */
    size_t size = (size_t)PERIOD + 1U;        /* indices 0 .. PERIOD */
    cache = (seq_t *)calloc(size, sizeof(seq_t));
    if (!cache)                      /* allocation failure */
        abort();

    /* Set the single initial '1' at index 17. */
    cache[17] = 1;

    /* Compute the remaining bits of the period using the recurrence. */
    for (size_t i = 18ULL; i <= PERIOD; ++i)
        cache[i] = cache[i - 7] ^ cache[i - 18];
}

/* The sequence function as required by the build. */
seq_t a(idx_t n)
{
    /* The sequence is periodic; reduce the argument if necessary. */
    n = n % PERIOD;

    /* Ensure the cache is ready. */
    init_cache();

    return cache[(size_t)n];
}
