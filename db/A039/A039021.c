/*
 * A039021  Numbers whose base-6 representation has the same number of 1's and
 *          3's.
 * https://oeis.org/A039021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑6 representation has the same number of 1's and 3's.
   This implementation builds the sequence on demand by simple enumeration,
   caching all terms that have been found so far. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* ensure n >= 1 (undefined behaviour otherwise) */
    if (n == 0) return 0;          /* defensive; not used in tests */

    static seq_t *buf = NULL;      /* cached sequence values           */
    static size_t buf_len = 0;     /* number of terms stored           */
    static size_t buf_cap = 0;     /* allocated capacity in terms       */
    static seq_t next_candidate = 0; /* smallest number not yet tested */

    /* grow cache until we have at least n terms */
    if (buf_len < (size_t)n) {
        /* initialise storage lazily */
        if (buf == NULL) {
            buf_cap = n * 2;               /* start with double the requested size */
            buf = (seq_t *)malloc(buf_cap * sizeof(seq_t));
            buf_len = 0;
            next_candidate = 0;
        }

        /* collect more valid terms until we reach n */
        while (buf_len < (size_t)n) {
            seq_t val = next_candidate;
            int c1 = 0, c3 = 0;

            /* count digits of val in base 6 */
            if (val == 0) {
                /* zero has 0 of each digit */
            } else {
                seq_t tmp = val;
                while (tmp) {
                    unsigned d = (unsigned)(tmp % 6);
                    if (d == 1) c1++;
                    else if (d == 3) c3++;
                    tmp /= 6;
                }
            }

            /* test the property */
            if (c1 == c3) {
                if (buf_len >= buf_cap) {
                    buf_cap = buf_cap * 2 + 16;   /* prevent frequent growth */
                    buf = (seq_t *)realloc(buf, buf_cap * sizeof(seq_t));
                }
                buf[buf_len++] = val;
            }

            next_candidate++;   /* move to next integer to test */
        }
    }

    return buf[n - 1];   /* array indices start at 0, a(1) is buf[0] */
}
