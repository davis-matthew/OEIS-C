/*
 * A161154  Positive integers n such that both {the number of (non-leading)
 *          0's in the binary representation of n} is coprime to n and {the
 *          number of 1's in the binary representation of n} is coprime to n.
 * https://oeis.org/A161154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bitlen_seq_t, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the n‑th term of OEIS sequence A161154.
 *  A number m is accepted if the count of its binary ones and the
 *  count of its non‑leading zeros are both coprime with m.
 *  The sequence is generated on demand by scanning natural numbers
 *  and storing the accepted values in a static array.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/bitlen_seq_t.h"

#define MAX_TERMS 20000   /* sufficient for n up to about 10000 */

static seq_t seq_arr[MAX_TERMS];
static size_t seq_len = 0;          /* number of terms currently stored */
static idx_t  last_checked = 0;     /* last integer examined */

/* Return true iff x satisfies the coprimality conditions. */
static inline bool is_candidate(seq_t x)
{
    seq_t ones  = 0;
    seq_t tmp   = x;
    while (tmp > 0) {
        ones += tmp & 1;
        tmp >>= 1;
    }

    seq_t bits  = bitlen_seq_t(x);
    seq_t zeros = bits - ones;   /* count of non‑leading zeros */

    /* zeros == 0 occurs only for x == 1; by definition 0 is coprime only to 1 */
    if (zeros == 0)
        return x == (seq_t)1;

    if (gcd(zeros, x) != (seq_t)1)
        return false;
    if (gcd(ones,  x) != (seq_t)1)
        return false;

    return true;
}

/* Return the n‑th term (1‑based index) of the sequence. */
seq_t a(idx_t n)
{
    /* Grow the cache until we have at least n terms. */
    while (seq_len < (size_t)n) {
        ++last_checked;
        seq_t val = (seq_t)last_checked;
        if (is_candidate(val)) {
            if (seq_len >= MAX_TERMS)  /* safety check; should not happen */
                break;
            seq_arr[seq_len++] = val;
        }
    }
    return seq_arr[(size_t)n - 1];
}
