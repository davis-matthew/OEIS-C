/*
 * A063302  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          29 ).
 * https://oeis.org/A063302
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 46, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A063302
 *  a(n) = a(n-4) + a(n-6) - a(n-10)   for n > 13
 *  a(2) = –1,  a(3) = 22,  a(4) = 56,  a(5) = 91,
 *  a(6) = 126, a(7) = 161, a(8) = 196, a(9) = 231,
 *  a(10)= 266, a(11)= 301, a(12)= 336, a(13)= 369
 *
 *  The function below memoises the sequence in a dynamic array.
 *  It works for any seq_t type that can hold the sequence’s values
 *  (including arbitrary‑precision implementations).  No other
 *  helper libraries are required.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *mem = NULL;          /* dynamic cache of values */
static size_t mem_used = 0;        /* first unused index (size of valid data) */
static size_t mem_cap  = 0;        /* allocated capacity */

/* Initialise the cache with the first 14 elements (indices 0 … 13).
 * 0 and 1 are unused, 2..13 hold the prescribed initial terms. */
static void init_cache(void)
{
    if (mem_used > 0) return;             /* already initialised */

    /* Allocate a small array that can hold indices 0…13. */
    mem_cap = 16;
    mem = (seq_t *)malloc(mem_cap * sizeof(seq_t));
    if (!mem) return;                      /* allocation failed – undefined */

    /* validate that the allocated capacity is large enough */
    if (mem_cap < 14) {
        mem_cap = 14;
        mem = (seq_t *)realloc(mem, mem_cap * sizeof(seq_t));
        if (!mem) return;
    }

    /* 0‑indexed array – indices < 2 never used by a(n) */
    mem[0] = (seq_t)0;
    mem[1] = (seq_t)0;

    mem[2] = (seq_t)-1;
    mem[3] = (seq_t)22;
    mem[4] = (seq_t)56;
    mem[5] = (seq_t)91;
    mem[6] = (seq_t)126;
    mem[7] = (seq_t)161;
    mem[8] = (seq_t)196;
    mem[9] = (seq_t)231;
    mem[10] = (seq_t)266;
    mem[11] = (seq_t)301;
    mem[12] = (seq_t)336;
    mem[13] = (seq_t)369;

    mem_used = 14;                         /* next free index */
}

/* Extend the cache if necessary and compute all missing values up to n. */
static void ensure_up_to(idx_t n)
{
    /* The sequence starts at n = 2.  Nothing to do for smaller n. */
    if (n < 2) return;

    init_cache();                           /* make sure base values exist */

    /* Grow the array to accommodate index n. */
    if ((size_t)n >= mem_cap) {
        size_t new_cap = mem_cap ? mem_cap * 2 : 16;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 16;
        seq_t *tmp = (seq_t *)realloc(mem, new_cap * sizeof(seq_t));
        if (tmp) {
            mem = tmp;
            mem_cap = new_cap;
        }   /* else keep old capacity – new indices will remain 0 */
    }

    /* Compute any missing terms using the recurrence. */
    for (size_t i = mem_used; i <= (size_t)n; ++i) {
        if (i <= 13) {          /* indices 2..13 already set */
            if (i < 2) mem[i] = (seq_t)0;   /* safety for 0,1 */
            continue;
        }
        mem[i] = mem[i - 4] + mem[i - 6] - mem[i - 10];
    }
    if ((size_t)n >= mem_used) mem_used = (size_t)n + 1;
}

/* --- public interface --------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* undefined below the offset */
    ensure_up_to(n);
    return mem[n];
}
