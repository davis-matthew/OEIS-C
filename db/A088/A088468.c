/*
 * A088468  a(0) = 1, a(n) = a(floor(n/2)) + a(floor(n/3)) for n > 0.
 * https://oeis.org/A088468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = a(n/2) + a(n/3) for n > 0 (floor division)
 * Range:     u8 n <= 647, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo   = NULL;   /* dynamic array of computed values      */
static char *filled  = NULL;   /* 1 if memo[i] is valid, 0 otherwise   */
static idx_t memo_size = 0;    /* current capacity (number of slots)    */

/* Ensure the tables can hold indices up to 'n' inclusive.  Parts of
 * the array that are newly allocated are marked as 'unfilled'.
 */
static void ensure_size(idx_t n)
{
    if (n + 1 <= memo_size) return;

    idx_t new_size = memo_size ? memo_size << 1 : 16;
    while (new_size <= n) new_size <<= 1;

    void *tmp_memo   = realloc(memo,   new_size * sizeof(seq_t));
    void *tmp_filled = realloc(filled, new_size * sizeof(char));
    if (!tmp_memo || !tmp_filled) exit(EXIT_FAILURE); /* out of memory */

    memo   = (seq_t *)tmp_memo;
    filled = (char   *)tmp_filled;

    /* Mark the newly allocated slots as unfilled.  The values in
     * memo[] are uninitialised; they will be written only when
     * filled[i] is set to 1.
     */
    for (idx_t i = memo_size; i < new_size; ++i) filled[i] = 0;

    memo_size = new_size;
}

/* Recursive helper that uses memoisation. */
static seq_t compute_rec(idx_t n)
{
    if (n == 0) return 1;

    ensure_size(n);
    if (filled[n]) return memo[n];

    seq_t val = compute_rec(n / 2) + compute_rec(n / 3);
    memo[n]   = val;
    filled[n] = 1;
    return val;
}

/* Public entry point required by the build. */
seq_t a(idx_t n)
{
    if (memo_size == 0) {          /* first call: initialise a(0) */
        ensure_size(0);
        memo[0]   = 1;
        filled[0] = 1;
    }
    return compute_rec(n);
}
