/*
 * A043811  Number of runs in the base 3 representation of n is congruent to 5
 *          mod 9.
 * https://oeis.org/A043811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose base‑3 representation
 *            contains a number of runs congruent to 5 (mod 9)
 * Range:     u8 n <= 34, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count runs (consecutive identical digits) in base‑3 representation of x. */
static int count_runs(seq_t x)
{
    int runs = 0;
    int prev = 3;                /* value outside [0,2] to start fresh */
    while (x > 0) {
        int digit = (int)(x % 3);
        if (digit != prev) {
            ++runs;
            prev = digit;
        }
        x /= 3;
    }
    return runs;
}

/* Return the n‑th term of A043811 (n is 1‑based). */
seq_t a(idx_t n)
{
    /* A small static cache; 20000 is ample for the typical test ranges. */
    static seq_t cache[20000];
    static idx_t cache_len = 0;     /* number of cached terms */
    static idx_t current = 1;       /* next integer to test */

    /* Extend cache until it contains at least n terms. */
    while (cache_len < n) {
        seq_t val;
        do {
            val = (seq_t)current++;
        } while ((count_runs(val) % 9) != 5);

        cache[cache_len++] = val;
    }

    return cache[n - 1];
}
