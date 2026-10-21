/*
 * A092808  Pair reversal of Jacobsthal numbers.
 * https://oeis.org/A092808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pair reversal of Jacobsthal numbers – OEIS A092808
   Recurrence (used in code):
       a(0) = 1, a(1) = 0
       for k ≥ 1
           a(2k)   = a(2k-2) + 2 * 4^(k-1)
           a(2k+1) = a(2k-1) +   4^(k-1)
   The function memoises the sequence and the powers of 4 so that each
   term is computed in O(1) after the required range has been expanded.
   It works for any seq_t / idx_t supplied by the build, including
   64‑bit, 128‑bit or arbitrary‑precision types. */

#include <stdlib.h>

/* forward declaration of helper – valid in C11 and C++17 */
static void ensure_pow4(idx_t);

static seq_t *seq      = NULL;  /* cached sequence values   */
static idx_t  seq_cap  = 0;     /* number of elements in seq */
static seq_t *pow4     = NULL;  /* powers of 4: pow4[i] = 4^i */
static idx_t  pow4_cap = 0;     /* allocated size of pow4   */

static void
ensure_pow4(idx_t idx)
{
    while (pow4_cap <= idx) {
        seq_t nxt = pow4[pow4_cap - 1] * (seq_t)4;
        pow4 = (seq_t *)realloc(pow4, sizeof(seq_t) * (pow4_cap + 1));
        pow4[pow4_cap++] = nxt;
    }
}

seq_t a(idx_t n)
{
    /* initialise pow4[0] = 1 */
    if (pow4 == NULL) {
        pow4_cap = 1;
        pow4 = (seq_t *)malloc(sizeof(seq_t) * pow4_cap);
        pow4[0] = (seq_t)1;
    }

    /* initialise sequence cache with the first two terms */
    if (seq == NULL) {
        seq_cap = 2;
        seq = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
        seq[0] = (seq_t)1;   /* a(0) */
        seq[1] = (seq_t)0;   /* a(1) */
    }

    /* expand the cache up to n */
    while (seq_cap <= n) {
        idx_t i = seq_cap;      /* next index to compute */
        seq_t value;

        if (i == 0 || i == 1) { /* these cases are already set */
            value = seq[i];
        } else {
            idx_t k = i / 2;               /* k = floor(i/2) */
            idx_t idx_pow = k - 1;         /* exponent of 4 needed */

            ensure_pow4(idx_pow);
            seq_t add = pow4[idx_pow];     /* 4^(k-1) */

            if ((i & 1) == 0)              /* even: 2k case */
                add = add * (seq_t)2;      /* multiply by 2 */

            value = seq[i - 2] + add;     /* recurrence */
        }

        seq = (seq_t *)realloc(seq, sizeof(seq_t) * (seq_cap + 1));
        seq[seq_cap++] = value;
    }

    return seq[n];
}
