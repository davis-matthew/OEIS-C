/*
 * A034041  Both primitively and imprimitively represented by (x^2+xy+2y^2,
 *          x>=0, y>=0).
 * https://oeis.org/A034041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number represented by x^2+xy+2y^2 both primitively (gcd(x,y)=1)
   and imprimitive (gcd(x,y)>1).  The function pre–computes all
   such numbers up to a growing bound and returns the nth term.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global state ------------------------------------------------------- */
static unsigned char *prim_flag = NULL;   /* 1 if a value has a primitive rep */
static unsigned char *impr_flag = NULL;   /* 1 if a value has an imprimitive rep */
static seq_t *both_vals = NULL;           /* ascending list of terms */
static size_t both_size = 0, both_cap = 0;
static idx_t processed_limit = 0;         /* largest value processed so far */
static bool initialized = false;

/* Simple Euclidean GCD for idx_t ----------------------------------- */
static inline idx_t my_gcd(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Recompute all flags and the list up to a new limit ---------------- */
static void recompute(idx_t limit)
{
    /* Allocate flag arrays */
    prim_flag = (unsigned char*)calloc(limit + 1, sizeof(unsigned char));
    impr_flag = (unsigned char*)calloc(limit + 1, sizeof(unsigned char));
    if (!prim_flag || !impr_flag) abort();

    /* Enumerate all (x,y) pairs producing a value <= limit */
    for (idx_t y = 0; 2 * y * y <= limit; ++y) {
        idx_t yy = 2 * y * y;
        for (idx_t x = 0; ; ++x) {
            idx_t val = x * x + x * y + yy;
            if (val > limit) break;
            if (val == 0) continue;
            idx_t g = my_gcd(x, y);
            if (g == 1)
                prim_flag[val] = 1;
            else
                impr_flag[val] = 1;
        }
    }

    /* Build the list of numbers that are both primitively & imprimitive */
    if (both_vals) {
        free(both_vals);
        both_vals = NULL;
    }
    both_size = 0;
    both_cap = 0;
    for (idx_t v = 1; v <= limit; ++v) {
        if (prim_flag[v] && impr_flag[v]) {
            if (both_size >= both_cap) {
                both_cap = both_cap ? both_cap * 2 : 1024;
                both_vals = (seq_t*)realloc(both_vals, both_cap * sizeof(seq_t));
                if (!both_vals) abort();
            }
            both_vals[both_size++] = (seq_t)v;
        }
    }

    processed_limit = limit;
    initialized = true;
}

/* Public sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) {
        recompute(200000);           /* initial bound large enough for ~10 000 terms */
    }
    if ((idx_t)n >= both_size) {
        recompute(processed_limit * 2);  /* extend bound if more terms requested */
    }
    return both_vals[n];
}
