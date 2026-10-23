/*
 * A108737  Start with S = {}. For m = 0, 1, 2, 3, ... let u be the binary
 *          expansion of m. If u is not a substring of S, append the minimal
 *          number of 0's and 1's to S to remedy this. Sequence gives S.
 * https://oeis.org/A108737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A108737 – Binary Champernowne in base 2.
   Start with empty string S.  For m = 0,1,2,… write u = binary expansion of m.
   If u is not a substring of S, append to S the shortest suffix that makes u
   appear (by maximizing overlap of the suffix of S with the prefix of u).
   a(n) is the n‑th character of the resulting infinite string (1‑based).   */

#include <stdint.h>
#include <stdlib.h>

/* external linkage – the function the test harness calls */
seq_t a(idx_t n)
{
    /* static state to memoise the construction across calls */
    static uint8_t *bits = NULL;      /* array of 0/1 bits */
    static size_t bits_cap = 0;       /* allocated capacity */
    static size_t bits_size = 0;      /* current number of bits */
    static idx_t next_m = 0;          /* next m to process */

    /* initialise only once */
    if (bits == NULL) {
        bits_cap = 1024;
        bits = (uint8_t *)malloc(bits_cap);
    }

    /* extend construction until we have at least n bits available */
    while (bits_size < (size_t)n) {
        /* compute binary representation of next_m */
        idx_t m = next_m;
        int len;
        uint8_t u[64];  /* 64 at most for uint64_t */
        if (m == 0) {
            len = 1;
            u[0] = 0;
        } else {
            idx_t tmp = m;
            len = 0;
            /* find number of bits */
            do { tmp >>= 1; len++; } while (tmp);
            /* fill array MSB first */
            for (int i = len - 1; i >= 0; --i) {
                u[i] = m & 1;
                m >>= 1;
            }
        }

        /* check whether u is already a substring of bits */
        int found = 0;
        if ((size_t)len <= bits_size) {
            for (size_t i = 0; i <= bits_size - (size_t)len; ++i) {
                int ok = 1;
                for (int j = 0; j < len; ++j) {
                    if (bits[i + j] != u[j]) { ok = 0; break; }
                }
                if (ok) { found = 1; break; }
            }
        }

        if (!found) {
            /* find maximal overlap between suffix of bits and prefix of u */
            size_t kmax = bits_size < (size_t)len ? bits_size : (size_t)len;
            int k;
            for (k = (int)kmax; k > 0; --k) {
                int ok = 1;
                for (int j = 0; j < k; ++j) {
                    if (bits[bits_size - k + j] != u[j]) { ok = 0; break; }
                }
                if (ok) break;
            }
            /* append the remaining part of u */
            for (int j = k; j < len; ++j) {
                if (bits_size == bits_cap) {
                    size_t new_cap = bits_cap * 2;
                    if (new_cap < bits_cap + 1) new_cap = bits_cap + 1;
                    bits = (uint8_t *)realloc(bits, new_cap);
                    bits_cap = new_cap;
                }
                bits[bits_size++] = u[j];
            }
        }
        ++next_m;   /* advance to next m for subsequent calls */
    }

    /* return the requested bit as seq_t */
    return (seq_t)bits[(size_t)n - 1];
}
