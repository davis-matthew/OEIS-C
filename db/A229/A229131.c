/*
 * A229131  Numbers k such that the distance between the k-th triangular
 *          number and the nearest square is exactly 1.
 * https://oeis.org/A229131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence: Numbers k such that the k‑th triangular number is a square
    ±1.  Computed by generating solutions of Pell equations
    x² – 8y² = 9  and  x² – 8y² = –7,
    where x = 2k+1 and y is the corresponding square root.
    Three independent solution families are used:
        (x, y) = (3,0)   (seed for x²–8y²= 9)
        (x, y) = (5,2)   (seed for x²–8y²= –7)
        (x, y) = (11,4)  (seed for x²–8y²= –7)
    Each family is multiplied by (3+√8) to generate the next solution.
    The resulting k = (x–1)/2 values are merged in ascending order. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct { seq_t x; seq_t y; } family_t;

static bool initialized = false;
static family_t fam[3];

static seq_t *values = NULL;
static size_t values_cap = 0;
static size_t values_len = 0;

/* Advance one step in the Pell recurrence:
 * (x + y√8) → (x + y√8)(3 + √8) = (3x + 8y) + (x + 3y)√8
 */
static inline void advance_family(family_t *f)
{
    seq_t nx = (seq_t)3 * f->x + (seq_t)8 * f->y;
    seq_t ny = f->x + (seq_t)3 * f->y;
    f->x = nx;
    f->y = ny;
}

/* Grow the dynamic array of stored a(n) values. */
static void grow_values(void)
{
    if (values_len >= values_cap)
    {
        size_t new_cap = values_cap ? values_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        values = tmp;
        values_cap = new_cap;
    }
}

/* Ensure data structures are initialized and compute up to n. */
static void init_and_compute(idx_t n)
{
    if (!initialized)
    {
        fam[0].x = 3; fam[0].y = 0;          /* seed for x²–8y² =  9 */
        fam[1].x = 5; fam[1].y = 2;          /* seed for x²–8y² = -7 */
        fam[2].x = 11; fam[2].y = 4;         /* seed for x²–8y² = -7 */
        initialized = true;
    }

    while (values_len < (size_t)n)
    {
        seq_t best_k = 0;
        int best_i = -1;
        for (int i = 0; i < 3; ++i)
        {
            seq_t k = (fam[i].x - (seq_t)1) / (seq_t)2;
            if (best_i == -1 || k < best_k)
            {
                best_k = k;
                best_i = i;
            }
        }
        grow_values();
        values[values_len++] = best_k;
        advance_family(&fam[best_i]);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; undefined for n<1 */
    init_and_compute(n);
    return values[n - 1];
}
