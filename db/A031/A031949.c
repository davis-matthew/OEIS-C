/*
 * A031949  Numbers with exactly two distinct base-4 digits.
 * https://oeis.org/A031949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A031949: Numbers with exactly two distinct base‑4 digits.
   The implementation counts how many such numbers exist for a given
   length L, locates the length of the nth term, then lexicographically
   selects the k‑th number of that length using combinatorial enumeration. */
#include <stdlib.h>

/* Pow‑2 cache: pow2[i] = 2^i as seq_t.  The cache is enlarged lazily. */
static seq_t *pow2_cache = NULL;
static size_t pow2_size = 0;

/* Ensure pow2_cache contains powers of two up to exponent 'needed'. */
static void ensure_pow2(size_t needed)
{
    if (pow2_size > needed)
        return;

    size_t new_size = needed + 1;
    seq_t *new_cache = (seq_t *)realloc(pow2_cache, new_size * sizeof(seq_t));
    if (!new_cache)
        exit(1); /* Allocation failed */

    /* Initialise missing entries.  The 0th entry is always 1. */
    if (pow2_size == 0) {
        new_cache[0] = (seq_t)1;
        pow2_size = 1;
    }

    for (size_t i = pow2_size; i <= needed; ++i)
        new_cache[i] = new_cache[i - 1] * (seq_t)2;

    pow2_cache = new_cache;
    pow2_size = new_size;
}

/* Count number of set bits in a 4‑bit mask. */
static inline int popcount_mask(unsigned int m)
{
    return __builtin_popcount(m);
}

/* Return the nth term (n >= 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* Undefined for n < 1 */

    seq_t n_seq = (seq_t)n;          /* Convert idx_t to seq_t for calculations. */
    seq_t cumulative = (seq_t)0;     /* Total numbers counted so far. */
    size_t L = 2;                    /* Minimum length of a base‑4 number (two digits). */

    /* Find the length L such that the nth term lies among numbers of length L. */
    while (1) {
        ensure_pow2(L - 1);
        seq_t pow = pow2_cache[L - 1];
        seq_t cntL = (seq_t)9 * (pow - (seq_t)1); /* Count for length L. */
        if (cumulative + cntL >= n_seq)
            break;          /* Found the correct length. */
        cumulative += cntL;
        ++L;
    }

    /* Zero‑based index within the block of length L. */
    seq_t k = n_seq - cumulative - (seq_t)1;

    unsigned char digits[128];   /* Holds the base‑4 digits of the result. */
    unsigned int used_mask = 0;   /* Bitmask of digits seen so far. */

    for (size_t pos = 0; pos < L; ++pos) {
        size_t rem = L - pos - 1;          /* Remaining positions after this. */
        int digit_start = (pos == 0) ? 1 : 0; /* Leading digit cannot be 0. */

        for (int d = digit_start; d <= 3; ++d) {
            unsigned int new_mask = used_mask | (1u << d);
            if (popcount_mask(new_mask) > 2)
                continue;                 /* Would introduce a third distinct digit. */

            int p = popcount_mask(new_mask);
            seq_t cnt;
            if (p == 1) {
                /* We still need to introduce a second distinct digit somewhere. */
                cnt = (seq_t)3 * (pow2_cache[rem] - (seq_t)1);
            } else /* p == 2 */ {
                /* Only two digits are allowed; any sequence of the remaining
                   positions using the same two digits is acceptable. */
                cnt = pow2_cache[rem];
            }

            if (k < cnt) {
                digits[pos] = (unsigned char)d;
                used_mask = new_mask;
                break;                  /* Move to next position. */
            } else {
                k -= cnt;                /* Skip these many numbers. */
            }
        }
    }

    /* Convert the base‑4 digit array to its decimal value. */
    seq_t value = (seq_t)0;
    for (size_t i = 0; i < L; ++i)
        value = value * (seq_t)4 + (seq_t)digits[i];

    return value;
}
