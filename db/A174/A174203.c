/*
 * A174203  In the sequence A007088 of binary representations of n, delete the
 *          1st, 3rd, 5th, 7th, 9th etc occurrence of 0 and 1. Rewrite the
 *          bits left over in base 10.
 * https://oeis.org/A174203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base, less
 * Formula:   a(n) = binary value obtained from the binary representation of
 *            n+2 after deleting globally all odd‑occurrence 0s and all
 *            odd‑occurrence 1s. The remaining bits (original order) form a
 *            binary number, which is interpreted in base 10
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static state for memoisation */
static seq_t *b_values = NULL;          /* array b(k) = value after deletion for number k   */
static idx_t  max_k   = 0;              /* largest number k for which b(k) is computed     */
static uint64_t zero_cnt = 0;            /* global count of 0s seen so far                  */
static uint64_t one_cnt  = 0;            /* global count of 1s seen so far                  */
static bool     init_done = false;

/* compute binary value of n after the global deletions
   while updating the global counters zero_cnt and one_cnt      */
static seq_t
build_value(idx_t n)
{
    seq_t  val   = 0;
    idx_t  msb   = 0;                /* position of most significant bit.  n==0 -> msb=0 */
    /* find msb position (for n>0) */
    if (n > 0) {
        idx_t tmp = n;
        while (tmp >>= 1) ++msb;
    }

    for (idx_t pos = msb; ; --pos) {
        uint64_t bit = (n >> pos) & 1ULL;
        if (bit == 0) {
            ++zero_cnt;
            if ((zero_cnt & 1ULL) == 0) {   /* even occurrence -> keep */
                val = (val << 1) | 0;
            }
        } else { /* bit == 1 */
            ++one_cnt;
            if ((one_cnt & 1ULL) == 0) {    /* even occurrence -> keep */
                val = (val << 1) | 1;
            }
        }
        if (pos == 0) break;               /* msb reached */
    }
    return val;
}

/* ensure that values up to target_k are computed and cached */
static void
ensure_computed(idx_t target_k)
{
    if (!init_done) {
        /* initialise: compute b(0) first */
        b_values = (seq_t *)malloc((target_k + 1) * sizeof(seq_t));
        zero_cnt = 0; one_cnt = 0;
        for (idx_t k = 0; k <= target_k; ++k)
            b_values[k] = build_value(k);
        max_k = target_k;
        init_done = true;
    } else if (target_k > max_k) {
        b_values = (seq_t *)realloc(b_values, (target_k + 1) * sizeof(seq_t));
        for (idx_t k = max_k + 1; k <= target_k; ++k)
            b_values[k] = build_value(k);
        max_k = target_k;
    }
}

/* required function */
seq_t a(idx_t n)
{
    /* a(n) uses the value for number (n+2) after deletion */
    idx_t target_k = n + 2;
    ensure_computed(target_k);
    return b_values[target_k];
}
