/*
 * A102659  List of Lyndon words on {1,2} sorted first by length and then
 *          lexicographically.
 * https://oeis.org/A102659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 226, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A102659 – List of Lyndon words on {1,2} sorted by length then lexicographically.
    The function returns the n‑th word as an integer whose decimal digits are the
    letters 1 and 2.  The algorithm enumerates all binary strings of increasing
    length, checks for the Lyndon property (primitive and lexicographically minimal
    among its rotations), and yields the desired element.  The enumeration stops
    as soon as the n‑th word is found.  For n up to about 10 000 this runs far
    below the time limit.  The implementation uses only integer arithmetic. */

#include <stdbool.h>
#include <stdint.h>

/* Global arrays used by the Lyndon test – static to avoid per‑call allocation.
   Max length considered for n≤10000 is < 32; array size 64 is safe for all cases. */
static int bits[64];
static int pi[64];

/* Test whether the word encoded by 'mask' of length L (most significant bit
   corresponds to the first character) is a Lyndon word over alphabet {1,2}. */
static bool is_lyndon(uint64_t mask, int L)
{
    /* Build the word in bits[0..L-1] (0→'1', 1→'2') */
    for (int i = 0; i < L; ++i)
        bits[i] = (mask >> (L - 1 - i)) & 1U;

    /* Check primitiveness via failure function (KMP prefix). */
    int j = 0;
    pi[0] = 0;
    for (int i = 1; i < L; ++i) {
        while (j > 0 && bits[i] != bits[j]) j = pi[j - 1];
        if (bits[i] == bits[j]) ++j;
        pi[i] = j;
    }
    int period = L - pi[L - 1];
    if (period < L && L % period == 0)        /* not primitive */
        return false;

    /* Ensure it is minimal among all rotations. */
    for (int shift = 1; shift < L; ++shift) {
        for (int k = 0; k < L; ++k) {
            int a = bits[k];
            int b = bits[(k + shift) % L];
            if (a != b) {
                if (b < a) return false;      /* rotation smaller */
                break;                         /* rotation larger, next shift */
            }
        }
    }
    return true;
}

/* Public function: a(n) returns the n-th Lyndon word on {1,2} turned into a
   decimal integer with digits 1 and 2.  The index n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out‑of‑range calls */

    idx_t count = 0;
    for (int len = 1; ; ++len) {
        uint64_t total = (uint64_t)1 << len;   /* all words of this length */
        for (uint64_t mask = 0; mask < total; ++mask) {
            if (is_lyndon(mask, len)) {
                ++count;
                if (count == n) {
                    /* Convert bits to decimal integer: 0→'1', 1→'2' */
                    seq_t val = 0;
                    for (int i = 0; i < len; ++i) {
                        val = val * 10 + (seq_t)(bits[i] + 1);
                    }
                    return val;
                }
            }
        }
    }
}
