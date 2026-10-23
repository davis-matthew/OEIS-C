/*
 * A189439  Number of 7Xn array permutations with each element not moving, or
 *          moving one space N, SW or SE.
 * https://oeis.org/A189439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the 15‑term linear recurrence a(n) = 23*a(n-1) +
 *            26*a(n-2) -2256*a(n-3) +3272*a(n-4) +63936*a(n-5) -101752*a(n-6)
 *            -714624*a(n-7) +1041216*a(n-8) +3522752*a(n-9) -4444032*a(n-10)
 *            -7691264*a(n-11) +5545472*a(n-12) +11157504*a(n-13)
 *            -4947968*a(n-14) -4030464*a(n-15) Initial values: a(...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, abort */

static const seq_t SEEDS[15] = {
    1ULL,
    85ULL,
    1245ULL,
    26589ULL,
    493941ULL,
    9557077ULL,
    181540773ULL,
    3467525301ULL,
    66045131269ULL,
    1258981901653ULL,
    23988387057381ULL,
    457127606313333ULL,
    8710469146675973ULL,
    165979359956189589ULL,
    3162723963901817637ULL
};

static const int COEFFS[15] = {
    23,    /* a(n-1)  */
    26,    /* a(n-2)  */
    -2256, /* a(n-3)  */
    3272,  /* a(n-4)  */
    63936, /* a(n-5)  */
    -101752,/* a(n-6) */
    -714624,/* a(n-7) */
    1041216,/* a(n-8) */
    3522752,/* a(n-9) */
    -4444032,/* a(n-10) */
    -7691264,/* a(n-11) */
    5545472,/* a(n-12) */
    11157504,/* a(n-13) */
    -4947968,/* a(n-14) */
    -4030464 /* a(n-15) */
};

/* Dynamic cache for computed terms. */
static seq_t *CACHE = NULL;      /* pointer to array of a(n) */
static idx_t CAPACITY = 0;       /* size of CACHE (number of stored entries) */

/* Ensure that CACHE contains terms up to index n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n == 0 || n <= CAPACITY)
        return;

    /* Expand capacity: at least the new required size, and double for future use. */
    idx_t old_cap = CAPACITY;
    idx_t new_cap = (CAPACITY > 0) ? CAPACITY : 16;
    while (new_cap < n)
        new_cap <<= 1;   /* double */

    /* Allocate new array. */
    seq_t *new_arr = NULL;
#ifdef __cplusplus
    new_arr = new seq_t[new_cap];
#else
    new_arr = (seq_t *)malloc(sizeof(seq_t) * new_cap);
#endif
    if (!new_arr) abort();   /* out of memory */

    /* Copy existing values. */
    for (idx_t i = 0; i < old_cap; ++i)
        new_arr[i] = CACHE[i];

    /* Free old memory. */
#ifdef __cplusplus
    delete[] CACHE;
#else
    free(CACHE);
#endif
    CACHE = new_arr;
    CAPACITY = new_cap;

    /* Initialise the seed values if the array is big enough. */
    if (CAPACITY >= 15) {
        for (idx_t i = 0; i < 15; ++i)
            CACHE[i] = SEEDS[i];
    }

    /* Compute new terms starting after the old capacity.  Skip the seed
       positions (indices 0 .. 14) when the old capacity was smaller than 15. */
    idx_t start = old_cap;
    if (start < 15)
        start = 15;

    for (idx_t i = start; i < CAPACITY; ++i)
    {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (int k = 0; k < 15; ++k)
        {
            idx_t idx = i - 1 - (idx_t)k;   /* corresponds to a(n-k-1) */
            if (COEFFS[k] >= 0)
                sum_pos += CACHE[idx] * (seq_t)COEFFS[k];
            else
                sum_neg += CACHE[idx] * (seq_t)(-COEFFS[k]);
        }
        /* Final value: positive sum minus negative sum (guaranteed non‑negative). */
        seq_t val = sum_pos;
        val -= sum_neg;
        CACHE[i] = val;
    }
}

/* External function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for 0; return 0 for safety */

    ensure_cache(n);
    return CACHE[n - 1U];
}
