/*
 * A085129  Multiples of 6 which are 7-smooth.
 * https://oeis.org/A085129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*A002473(n) The sequence consists of the multiples of 6
 *            whose prime factors are all in {2,3,5,7}. We generate the
 *            7‑smooth numbers 2^i 3^j 5^k 7^ℓ (i,j,k,ℓ≥0) in increasing order
 *            using the classic multiplier‑array method, and keep those that
 *            are divisible by 6. The arrays are preallocated...
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <assert.h>

/* Preallocation limits – safe for n up to about 10 000.
   The numbers grow fast enough that 200 000 smooth values
   cover all needed multiples of 6. */
#define MAX_SMOOTH 200000
#define MAX_MULT   200000

/* Persistent storage, initialized on first call. */
static seq_t smooth[MAX_SMOOTH];
static seq_t multiples[MAX_MULT];
static size_t smooth_len = 0;  /* number of valid entries in smooth   */
static size_t multiples_len = 0;  /* number of valid entries in multiples */
static idx_t  p2 = 0, p3 = 0, p5 = 0, p7 = 0;  /* indices for next candidate */

/* Initialize static buffers on first invocation. */
static void init_buffers(void)
{
    if (smooth_len == 0) {
        smooth[0] = (seq_t)1;
        smooth_len = 1;
        /* multiples array starts empty */
        multiples_len = 0;
        /* indices start at 0 pointing to the initial value */
        p2 = p3 = p5 = p7 = 0;
    }
}

/* Generate enough terms to satisfy a(n). */
static void generate_multiples(size_t required)
{
    while (multiples_len < required) {
        /* Compute the next candidates for each prime multiplier. */
        seq_t cand2 = smooth[p2] * (seq_t)2;
        seq_t cand3 = smooth[p3] * (seq_t)3;
        seq_t cand5 = smooth[p5] * (seq_t)5;
        seq_t cand7 = smooth[p7] * (seq_t)7;

        /* Pick the smallest candidate. */
        seq_t next = cand2;
        if (cand3 < next) next = cand3;
        if (cand5 < next) next = cand5;
        if (cand7 < next) next = cand7;

        /* Advance all indices that produced this value 
           to avoid duplicate entries. */
        if (cand2 == next) ++p2;
        if (cand3 == next) ++p3;
        if (cand5 == next) ++p5;
        if (cand7 == next) ++p7;

        /* Add the new smooth number, checking capacity. */
        assert(smooth_len + 1 <= MAX_SMOOTH);
        smooth[smooth_len++] = next;

        /* If it is a multiple of 6, record it. */
        if ((next % (seq_t)6) == (seq_t)0) {
            assert(multiples_len + 1 <= MAX_MULT);
            multiples[multiples_len++] = next;
        }
    }
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1. */
    assert(n >= 1);

    init_buffers();               /* ensures buffers are ready */
    generate_multiples((size_t)n);/* fill as many multiples as needed */

    return multiples[n - 1];
}
