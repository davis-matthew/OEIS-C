/*
 * A133420  Number of steps to reach 1 under repeated applications of the
 *          "5x+1" map of A133419, or -1 if 1 is never reached.
 * https://oeis.org/A133420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 4894, u32 n <= 9026, u64 n <= 9026,
 *            u128 n <= 9026, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the number of steps needed for n to reach 1
   under the 5x+1 map:
      x -> x/2  if x even
      x -> x/3  if x divisible by 3
      x -> 5x+1 otherwise
   If the process enters a cycle that does not contain 1 the function
   returns (seq_t)-1.  The sequence is A133420.                                              */

#include <stdlib.h>   /* malloc, realloc, exit */

static seq_t *seen = NULL;     /* dynamic array of previously visited values   */
static size_t seen_cap = 0;    /* current capacity of 'seen'                   */
static size_t seen_len = 0;    /* number of values currently stored           */

/* Ensure that the seen array can hold at least 'needed' entries. */
static void ensure_seen_cap(size_t needed)
{
    if (seen_cap < needed) {
        size_t new_cap = seen_cap ? seen_cap * 2 : 256;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(seen, new_cap * sizeof(seq_t));
        if (!tmp)          /* allocation failure – abort the run */
            exit(1);
        seen = tmp;
        seen_cap = new_cap;
    }
}

/* Perform one step of the 5x+1 map on an unsigned integer x. */
static inline seq_t next_value(seq_t x)
{
    if ((x & 1) == 0)          /* x even → x/2 */
        return x >> 1;
    else if (x % 3 == 0)        /* divisible by 3 → x/3 */
        return x / 3;
    else                       /* otherwise 5x+1 */
    {
#ifdef __SIZEOF_INT128__
        /* For builds that support __int128, use it to avoid overflow
           of the intermediate computation.  The result is then cast
           back to seq_t. */
        return (seq_t)((unsigned __int128)x * 5 + 1);
#else
        /* If __int128 is not available, rely on the wrap‑around
           semantics of unsigned arithmetic; this will be correct for
           types that can accommodate the value without overflow. */
        return x * 5 + 1;
#endif
    }
}

/* Main sequence function required by the evaluator. */
seq_t a(idx_t n)
{
    if (n == 1)                 /* a(1) = 0 */
        return 0;

    seq_t x = (seq_t)n;
    seq_t steps = 0;
    seen_len = 0;               /* start a fresh visited list for this call */

    for (;;) {
        /* Linear search to detect a cycle. */
        for (size_t i = 0; i < seen_len; ++i)
            if (seen[i] == x)
                return (seq_t)-1;   /* cycle detected, 1 never reached */

        if (x == 1)
            return steps;          /* reached 1 */

        /* Record this value and advance. */
        ensure_seen_cap(seen_len + 1);
        seen[seen_len++] = x;

        x = next_value(x);
        ++steps;
    }
}
