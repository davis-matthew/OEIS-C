/*
 * A118176  a(1) = 1. a(n) is the number of earlier terms of the sequence
 *          which have a different number of divisors than n.
 * https://oeis.org/A118176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1. For n > 1: a(n) = (n-1) - |{ i < n :
 *            divisor_count(a(i)) == divisor_count(n) }|
 * Range:     u8 n <= 263, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9188 (2026-11-03)
 * Helpers:   mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/num_divisors.h"
#include "../utils/mem.h"

#if defined(__cplusplus)
  /* In C++ convert by C‑style cast; works for both integer types and
     arbitrary‑precision seq_t classes that provide a constructor from
     integer or a conversion operator to uint64_t. */
  #define TO_SEQ_T(x)      (seq_t)(x)
  #define TO_U64(x)        (uint64_t)(x)
#else
  /* In C ISA, cast to the target type.  seq_t is an integer type for C. */
  #define TO_SEQ_T(x)      ((seq_t)(x))
  #define TO_U64(x)        ((uint64_t)(x))
#endif

/* maximum divisor count we expect to see for the n values we handle;
   2048 safely exceeds the divisor count of any n <= 10^8. */
#define MAX_DIVCOUNT 2048

/* static storage for sequence values and divisor‑count frequencies */
static seq_t *seq_values = NULL;   /* 0‑based array of computed a(n) values */
static size_t seq_len   = 0;        /* number of values stored so far */
static size_t seq_cap   = 0;        /* capacity of seq_values */

static unsigned int divcnt_freq[MAX_DIVCOUNT] = {0};

/* grow seq_values to accommodate `n` entries */
static void ensure_capacity(size_t n)
{
    if (seq_cap < n) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        while (new_cap < n) new_cap *= 2;
        /* cast necessary for C++ */
        seq_values = mem_resize(seq_t, seq_values, new_cap);
        seq_cap = new_cap;
    }
}

/* ----------  OEIS sequence A118176  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return TO_SEQ_T(0);  /* undefined, but safe */

    /* generate terms up to n if needed */
    if (seq_len < n) {
        ensure_capacity(n);
        for (idx_t cur = seq_len + 1; cur <= n; ++cur) {

            /* term 1 is the special initial value 1 */
            if (cur == 1) {
                seq_values[seq_len++] = TO_SEQ_T(1);
                uint64_t d1 = num_divisors(TO_U64(1));
                if (d1 < MAX_DIVCOUNT) divcnt_freq[d1]++;
                continue;
            }

            /* divisor count of the index n itself */
            uint64_t d_cur = num_divisors(TO_U64(cur));
            unsigned int same = 0;
            if (d_cur < MAX_DIVCOUNT) same = divcnt_freq[d_cur];

            /* a(cur) = (cur-1) - same */
            uint64_t val_u64 = TO_U64(cur - 1) - (uint64_t)same;
            seq_t val = TO_SEQ_T(val_u64);
            seq_values[seq_len++] = val;

            /* update frequency table for divisor count of a(cur) */
            uint64_t d_val = num_divisors(val_u64);
            if (d_val < MAX_DIVCOUNT) divcnt_freq[d_val]++;
        }
    }
    return seq_values[n - 1];
}
