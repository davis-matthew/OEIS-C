/*
 * A225180  Infinite sequence M defined by the rules M = 1:X, X =
 *          2:zip_2(X,Y), Y = 2:zip_3(M,Y,Y).
 * https://oeis.org/A225180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A225180: a(n) is the nth term of the infinite word M defined by
   M = 1:X, X = 2:zip_2(X,Y), Y = 2:zip_3(M,Y,Y);
   The sequence consists of the values 1 and 2. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static size_t seq_cap = 0;           /* current allocation capacity   */
static size_t seq_len = 0;           /* number of elements computed so far */
static seq_t *M_seq = NULL;          /* sequence M */
static seq_t *X_seq = NULL;          /* sequence X */
static seq_t *Y_seq = NULL;          /* sequence Y */

/* Ensure that M_seq, X_seq and Y_seq are computed up to index n (inclusive). */
static void ensure_up_to(idx_t n)
{
    /* Initialize the sequences once. */
    if (seq_len == 0) {
        seq_cap = 64;                 /* initial capacity */
        M_seq = seq_alloc(seq_cap);
        X_seq = seq_alloc(seq_cap);
        Y_seq = seq_alloc(seq_cap);

        /* base initial elements */
        M_seq[0] = (seq_t)1;
        X_seq[0] = (seq_t)2;
        Y_seq[0] = (seq_t)2;

        seq_len = 1;                  /* index 0 is now available */
    }

    /* Grow the arrays if needed. */
    if ((idx_t)n >= seq_len) {
        size_t needed = (size_t)(n + 1);
        if (needed > seq_cap) {
            size_t newcap = seq_cap;
            while (newcap < needed) newcap <<= 1;
            M_seq = seq_realloc(M_seq, seq_cap, newcap);
            X_seq = seq_realloc(X_seq, seq_cap, newcap);
            Y_seq = seq_realloc(Y_seq, seq_cap, newcap);
            seq_cap = newcap;
        }

        /* Compute new elements one by one. */
        for (idx_t i = seq_len; i <= n; ++i) {
            idx_t k = i - 1;          /* index in the zip parts */

            /* X[i] = zip_2(X,Y)[k] */
            seq_t xv = (((k & 1) == 0) ? X_seq[k / 2] : Y_seq[(k - 1) / 2]);
            X_seq[i] = xv;

            /* Y[i] = zip_3(M,Y,Y)[k] */
            seq_t yv;
            idx_t mod3 = k % 3;
            if (mod3 == 0) {
                yv = M_seq[k / 3];
            } else if (mod3 == 1) {
                yv = Y_seq[(k - 1) / 3];
            } else {
                yv = Y_seq[(k - 2) / 3];
            }
            Y_seq[i] = yv;

            /* M[i] = X[i-1] for i >= 1 (index 0 already set) */
            M_seq[i] = X_seq[i - 1];
        }

        seq_len = n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return M_seq[n];
}
