/*
 * A090956  Numbers in increasing order such that successive sums are squares
 *          and successive differences are squarefree.
 * https://oeis.org/A090956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 134, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_is_squarefree, seq_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1.
 * For n > 0, a(n) is the least integer > a(n-1) satisfying:
 *   a(n) + a(n-1) is a perfect square,
 *   a(n) - a(n-1) is square‑free.
 * The sequence is computed lazily, memoised, and works for any seq_t
 * type (8‑64‑bit, 128‑bit or arbitrary‑precision).
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_squarefree.h"
#include "../utils/seq_isqrt.h"

/* -------------------------------------------------------------- */
/*  Integer square‑root for any seq_t dimension. */

/* -------------------------------------------------------------- */
/*  Check if a seq_t value is square‑free. */

/* -------------------------------------------------------------- */
/*  Storage for generated terms. */
/* -------------------------------------------------------------- */
static seq_t *seq_buf = NULL;   /* dynamic array of computed terms */
static idx_t  seq_len = 0;      /* number of terms stored */
static idx_t  seq_cap = 0;      /* allocated capacity */

static void seq_grow(idx_t min_cap)
{
    if (min_cap <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap << 1 : 1;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *new_buf = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
    seq_buf = new_buf;
    seq_cap = new_cap;
}

/* -------------------------------------------------------------- */
/*  Main sequence routine. */
/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Generate terms up to index n (inclusive). */
    if (seq_len <= n) {
        seq_grow(n + 1);
        while (seq_len <= n) {
            if (seq_len == 0) {            /* a(0) = 1 */
                seq_buf[0] = (seq_t)1;
                seq_len = 1;
                continue;
            }

            seq_t prev = seq_buf[seq_len - 1];

            /* Compute s = floor(sqrt(2*prev)) + 1  */
            seq_t twice = prev + prev;      /* 2*prev */
            seq_t s = seq_isqrt(twice) + 1;

            /* Find the first s with square‑free diff. */
            while (true) {
                seq_t square = s * s;      /* s^2 */
                seq_t diff   = square - twice;   /* difference = s^2 – 2*prev */

                if (seq_is_squarefree(diff)) {
                    seq_t cand = square - prev;   /* a(n) = s^2 – prev */
                    seq_buf[seq_len] = cand;
                    break;
                }
                ++s;
            }

            ++seq_len;
        }
    }
    return seq_buf[n];
}
