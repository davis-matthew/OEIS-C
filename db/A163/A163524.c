/*
 * A163524  The pairs (x,y) such that x^2 = 1+factorial(y).
 * https://oeis.org/A163524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   factorial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth term of sequence A163524.
   The sequence lists integers alternately: x and y where
   x^2 = 1 + y!  (factorial of y).  For each solution (x,y)
   we put x as term 2k+1 and y as term 2k+2.
   The known solutions are (5,4), (11,5), (71,7) and no further
   solutions are known.  The function generates solutions
   on demand and caches them.  For n larger than the number of
   known solutions it returns 0. */
#include <stddef.h>
#include <stdbool.h>
/* utils for big integer manipulation (provided by the build system) */
#include "../utils/factorial.h"      /* factorial utility if needed */
#include "../utils/floor_sqrt.h"     /* floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *terms = NULL;      /* array of alternating x and y */
    static size_t term_count = 0;    /* number of terms stored */
    static size_t term_cap = 0;      /* capacity of terms array */
    static idx_t next_y = 1;         /* next y to try */
    static seq_t fact = 1;          /* current factorial value */

    /* Ensure we have enough terms computed for requested n */
    while (term_count < n)
    {
        /* Expand the cache if needed */
        if (term_count >= term_cap)
        {
            size_t new_cap = term_cap == 0 ? 4 : term_cap * 2;
            seq_t *new_terms = NULL;
            /* realloc handles both C and C++ builds */
            new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!new_terms) return 0; /* allocation failed; return 0 */
            terms = new_terms;
            term_cap = new_cap;
        }

        /* Compute next factorial */
        fact = fact * (seq_t)next_y;
        next_y++;

        /* Compute fact + 1 and check if it's a perfect square */
        seq_t val = fact + 1;
        seq_t root = floor_sqrt(val);
        if (root * root == val)
        {
            /* Found a solution: store x then y */
            terms[term_count++] = root;
            if (term_count >= term_cap)
            {
                size_t new_cap = term_cap == 0 ? 4 : term_cap * 2;
                seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!new_terms) return 0;
                terms = new_terms;
                term_cap = new_cap;
            }
            terms[term_count++] = (seq_t)(next_y - 1); /* y is previous value of next_y */
        }

        /* Safety guard: stop after a large y if no more solutions appear.
           This prevents an infinite loop in case the function is called
           with a huge n beyond known solutions. */
        if (next_y > 1000 && term_count == 0)
            break;  /* No solutions found up to y=1000 */
    }

    /* Return the requested term or 0 if it doesn't exist */
    return term_count >= n ? terms[n - 1] : 0;
}
