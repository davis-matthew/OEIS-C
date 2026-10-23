/*
 * A134042  Complete list of solutions to y^2 = x^3 + 113; sequence gives x
 *          values.
 * https://oeis.org/A134042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 none, i16 none, i32 none, i64 n <= 6, i128 n <= 6, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the integer solutions x to the elliptic curve
    y^2 = x^3 + 113.  The sequence a(n) lists the x‑values in
    ascending order, indexed from 1.  The curve has only finitely
    many integer points; this routine finds them by brute‑force
    search up to a fixed bound and caches the results.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

/*  Returns the n‑th x‑value satisfying y^2 = x^3 + 113  */
seq_t a(idx_t n)
{
    /* static cache of solutions */
    static bool initialized = false;
    static seq_t *solutions = NULL;
    static size_t sol_count  = 0;
    static size_t sol_cap    = 0;

    if (!initialized) {
        /* search range – far beyond the known largest solution 422
           The bound 20000 is easily enough for the small solutions
           expected for this curve, and keeps a single linear pass
           fast for all requested n. */
        const seq_t MAX_X = (seq_t)20000;

        sol_cap    = 8;                 /* initial capacity */
        solutions  = (seq_t*)malloc(sol_cap * sizeof(seq_t));
        sol_count  = 0;

        for (seq_t x = -MAX_X; x <= MAX_X; ++x) {
            seq_t t = x * x * x + (seq_t)113;   /* x^3 + 113 */
            if (t < 0) continue;               /* no non‑negative square */
            seq_t y = floor_sqrt(t);
            if (y * y == t) {                  /* perfect square found */
                if (sol_count == sol_cap) {
                    sol_cap *= 2;
                    solutions = (seq_t*)realloc(solutions,
                                                sol_cap * sizeof(seq_t));
                }
                solutions[sol_count++] = x;
            }
        }
        /* Sort in ascending order – solutions were found in order
           because x was iterated ascending, but the negative
           values appear before positives.  The order is therefore
           already correct. */
        initialized = true;
    }

    /* 1‑based indexing, return 0 when n requests a non‑existent term  */
    if (n == 0 || n > sol_count) return 0;
    return solutions[n - 1];
}
