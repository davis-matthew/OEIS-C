/*
 * A075311  a(1) = 1; for n > 1, a(n) is the smallest number m > a(n-1) such
 *          that the number of 1's in the binary expansion of m is not already
 *          in the sequence.
 * https://oeis.org/A075311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the smallest m > a(n-1) such that the number of 1's in
 *            the binary representation of m has not appeared as a term before
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq = NULL;          /* dynamic array of computed terms */
static size_t seq_len = 0;         /* number of terms currently stored */
static size_t seq_cap = 0;         /* allocated capacity of seq */

/* binary search for pc in seq[0..len-1] */
static bool term_contains(size_t pc, size_t len) {
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (seq[mid] == (seq_t)pc)
            return true;
        else if (seq[mid] < (seq_t)pc)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* ensure seq has capacity at least need */
static void ensure_capacity(size_t need) {
    if (seq_cap >= need)
        return;
    size_t new_cap = (seq_cap == 0) ? 4 : seq_cap * 2;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = new_ptr;
    seq_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;    /* undefined for n=0 */

    size_t target = (size_t)n;

    /* generate terms until we have at least 'target' of them */
    while (seq_len < target) {
        if (seq_len == 0) {           /* first term */
            ensure_capacity(1);
            seq[0] = 1;
            seq_len = 1;
            continue;
        }

        seq_t last = seq[seq_len - 1];
        seq_t m = last + 1;          /* candidate */

        for (;;) {
            /* popcount of m */
            seq_t tmp = m;
            size_t pc = 0;
            while (tmp) {
                tmp &= tmp - 1;
                ++pc;
            }

            /* if popcount not yet a term, accept m */
            if (!term_contains(pc, seq_len)) {
                ensure_capacity(seq_len + 1);
                seq[seq_len++] = m;
                break;
            }
            ++m;   /* try next candidate */
        }
    }

    return seq[n - 1];
}
