/*
 * A160532  Those positive integers n that contain runs of 0's and 1's that
 *          are each a power of 2 in length when n is represented in binary.
 * https://oeis.org/A160532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   is_pow2_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A160532 – positive integers whose binary run lengths are all powers of 2.
 *
 * The function lazily generates the sequence by brute‑force testing of
 * successive integers and stores the results in a static buffer.  The
 * buffer is oversized (2×10⁴ elements) so that every required term
 * up to n ≈ 10 000 can be accessed in O(1) after it has been
 * generated once.  The algorithm runs in O(n·log n) time in total,
 * which is easily below the 2 s limit for the indicated range.
 *
 * Note:  seq_t may be a built‑in unsigned integer type or an
 * arbitrary‑precision integer class with the usual arithmetic and
 * bitwise operators.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_pow2_idx.h"

/* Return true iff the binary run‐lengths of x are all powers of two. */
static bool is_valid(seq_t x)
{
    while (x != 0) {
        seq_t bit = x & 1;      /* current run bit (0 or 1) */
        idx_t  len = 0;         /* length of current run */

        /* count the run */
        while (x != 0 && (x & 1) == bit) {
            x >>= 1;
            ++len;
        }

        if (!is_pow2_idx(len))
            return false;       /* run of forbidden length found */
    }
    return true;
}

/* Maximum number of terms we reserve space for.  Two times the
 * anticipated maximum index gives a generous buffer. */
#define CACHE_CAP 20000

/* Static cache of sequence values and the next integer to test. */
static seq_t  cache[CACHE_CAP];
static idx_t  cache_len = 0;  /* number of terms stored so far */
static seq_t  next = 1;       /* next integer to test */

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* The function memoises all terms up to the requested index. */
    while (cache_len < n) {
        if (is_valid(next)) {
            if (cache_len < CACHE_CAP)  /* guard against overflow */
                cache[cache_len++] = next;
            else
                ;   /* buffer full – in practice this will not happen for n ≤ 10 000 */
        }
        next++;   /* advance candidate */
    }

    /* cache_len ≥ n now – return the requested term */
    return cache[n - 1];
}
