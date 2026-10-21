/*
 * A239017  List of primitive words on {1,2,3}.
 * https://oeis.org/A239017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primitive words over {1,2,3} enumerated lexicographically by
   increasing word length.  The function generates the nth word by
   brute‑force enumeration, skipping non‑primitive (repeated) words. */

#include <stdbool.h>
#include <stdint.h>

/* Return the n-th primitive word over {1,2,3}.  Words are interpreted as
   base‑10 integers formed by the digits 1–3.  The enumeration order is
   lexicographic: all words of length 1, then length 2, etc.  */
seq_t a(idx_t n) {
    /* Keep a counter of how many primitive words have been seen so far. */
    idx_t seen = 0;

    /* For safety, use a sufficiently large array for at most 64 digits
       (more than enough for any practical n in this problem). */
    unsigned char digits[64];
    for (idx_t len = 1; ; ++len) {               /* word length */

        /* Compute 3^len as a 64‑bit integer.  For len beyond 20 this
           would overflow, but such a length is never needed for n up to
           10 000 or larger practical queries. */
        uint64_t pow3 = 1;
        for (idx_t k = 0; k < len; ++k) pow3 *= 3;

        for (uint64_t idx = 0; idx < pow3; ++idx) {
            /* Convert idx to base‑3 representation with exactly 'len'
               digits (most‑significant digit first). */
            uint64_t t = idx;
            for (int pos = len - 1; pos >= 0; --pos) {
                digits[pos] = t % 3;   /* digits 0,1,2 for letters 1,2,3 */
                t /= 3;
            }

            /* Check primitiveness: a word is non‑primitive if its minimal
               period d < len divides len and the word repeats every d
               characters.  We test all divisors d of len. */
            bool primitive = true;
            for (idx_t d = 1; d < len; ++d) {
                if (len % d != 0) continue;   /* d must divide len */

                bool periodic = true;
                for (idx_t pos = d; pos < len; ++pos) {
                    if (digits[pos] != digits[pos - d]) {
                        periodic = false;
                        break;
                    }
                }
                if (periodic) {  /* found a smaller period */
                    primitive = false;
                    break;
                }
            }

            if (!primitive) continue;   /* skip non‑primitive words */

            ++seen;
            if (seen == n) {  /* construct the integer value to return */
                seq_t val = 0;
                for (idx_t pos = 0; pos < len; ++pos) {
                    val = val * 10 + (seq_t)(digits[pos] + 1);  /* digits 1–3 */
                }
                return val;
            }
        }
        /* If we exited the inner loop without returning, we need a
           longer word; the outer loop will increase len. */
    }
}
