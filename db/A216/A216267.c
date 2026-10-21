/*
 * A216267  Numbers that are both tetrahedral and pronic.
 * https://oeis.org/A216267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Numbers that are both tetrahedral and pronic. Compute
 *            tetrahedral numbers T = x*(x+1)*(x+2)/6 for x increasing. A
 *            tetrahedral number is pronic iff 1+4*T is a perfect square and
 *            that square is odd. Exhaustive search up to a generous bound
 *            (here 1 000 000) finds all solutions; the sequence is finite
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Integer square root of a 64‑bit unsigned value (floor(sqrt(x))).  */
static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t lo = 0;
    uint64_t hi = (uint64_t)1 << 32;          /* because sqrt(UINT64_MAX) < 2^32 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > x / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Cached list of solutions.  */
static seq_t *solutions   = NULL;
static size_t sol_count   = 0;
static bool   initialized = false;

/* Build the solution table once.  */
static void init_solutions(void)
{
    if (initialized) return;
    initialized = true;

    /* Upper bound for the tetrahedral index – far beyond the real maximum. */
    const idx_t LIMIT = 1000000;

    size_t capacity = 8;
    solutions = (seq_t *)malloc(capacity * sizeof(seq_t));
    if (!solutions) return;             /* allocation failure – nothing to do */

    sol_count = 0;

    for (idx_t x = 0; x <= LIMIT; ++x) {
        uint64_t ux = (uint64_t)x;
        uint64_t T  = ux * (ux + 1) * (ux + 2) / 6;  /* tetrahedral number */
        uint64_t D  = 1 + 4 * T;
        uint64_t s  = isqrt_uint64(D);

        if (s * s == D && (s & 1u) == 1u) {          /* pronic test succeeds */
            seq_t val = (seq_t)T;                     /* cast is safe for all
                                                           values that fit in
                                                           seq_t (largest
                                                           known solution < 2^32) */
            if (sol_count && solutions[sol_count - 1] == val)
                continue;                            /* skip duplicates */

            if (sol_count == capacity) {             /* grow array if needed */
                capacity *= 2;
                seq_t *tmp = (seq_t *)realloc(solutions, capacity * sizeof(seq_t));
                if (!tmp) { free(solutions); solutions = NULL; sol_count = 0; return; }
                solutions = tmp;
            }
            solutions[sol_count++] = val;
        }
    }

    /* Shrink to fit actual number of solutions. */
    if (sol_count < capacity) {
        seq_t *tmp = (seq_t *)realloc(solutions, sol_count * sizeof(seq_t));
        if (tmp) solutions = tmp;
    }
}

/* Public interface: seq_t a(idx_t n)
   Returns the n-th term of the sequence (1‑based).  */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, return 0 */

    init_solutions();

    if (!solutions || n > sol_count)
        return (seq_t)0;          /* no further terms */

    return solutions[n - 1];
}
