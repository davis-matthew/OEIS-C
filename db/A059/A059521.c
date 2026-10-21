/*
 * A059521  5, followed by the digits of the cubes of each term.
 * https://oeis.org/A059521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) consists of the decimal digits of the cubes of preceding
 *            terms, starting with 5. Sequence A059521
 * Range:     u8 n <= 10, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *seq = NULL;       /* dynamic array of sequence terms */
static size_t seq_size = 0;     /* current number of terms stored */
static size_t seq_cap = 0;      /* allocated capacity of seq */
static idx_t next_k = 0;        /* index of the next term to process (0‑based) */

/* helper to grow the array to hold at least 'add' more elements */
static void grow_seq(size_t add)
{
    if (seq_size + add <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < seq_size + add)
        new_cap <<= 1;
    if (seq == NULL) {
        seq = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    }
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined, but safe guard */
    size_t target = (size_t)n;

    /* initialize the first term if not already done */
    if (seq_size == 0) {
        grow_seq(1);
        seq[0] = 5;              /* a(1) = 5 */
        seq_size = 1;
        next_k = 0;              /* process the cube of seq[0] next */
    }

    /* generate terms until we have at least 'target' terms */
    while (seq_size < target) {
        /* process the cube of seq[next_k] */
        seq_t cube = seq[next_k] * seq[next_k] * seq[next_k];

        /* extract digits of 'cube' (most significant first) */
        seq_t digits[50];       /* enough for up to 128‑bit number */
        size_t d = 0;
        if (cube == 0) {
            digits[d++] = 0;
        } else {
            while (cube > 0) {
                digits[d++] = cube % 10;
                cube /= 10;
            }
        }
        /* append digits in reverse order (MSD first) */
        grow_seq(d);
        for (size_t i = 0; i < d; ++i) {
            seq[seq_size] = digits[d - 1 - i];
            ++seq_size;
        }

        ++next_k;               /* move to next term to process */
    }

    return seq[target - 1];
}
