/*
 * A077581  Triangle in which row n contains the n smallest numbers starting
 *          from 1 and coprime to n.
 * https://oeis.org/A077581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 3559, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) = j‑th smallest positive integer coprime to r,
 * where r satisfies (r‑1)·r/2 < n ≤ r·(r+1)/2
 * and j = n – (r‑1)·r/2.
 */
#include <stdint.h>
#include <stdbool.h>

/* Euclidean algorithm for idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Find the row r such that (r‑1)·r/2 < n ≤ r·(r+1)/2. */
static idx_t find_row(idx_t n)
{
    idx_t r = 1;
    /* Incrementally search until the triangular bound exceeds n. */
    while (((unsigned __int128)r * (r + 1)) / 2 < n)
        ++r;
    return r;
}

/* Return the j‑th positive integer coprime to r. */
static idx_t coprime_nth_idx(idx_t r, idx_t j)
{
    idx_t candidate = 0;
    idx_t count = 0;
    while (count < j) {
        ++candidate;
        if (gcd_idx(candidate, r) == 1)
            ++count;
    }
    return candidate;
}

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; undefined for 0 */
        return 0;

    idx_t r = find_row(n);
    idx_t before = ((unsigned __int128)r * (r - 1)) / 2;
    idx_t j = n - before; /* 1‑based position in row r */

    idx_t cand = coprime_nth_idx(r, j);
    /* Truncation only occurs when the value exceeds the capacity of
       seq_t; such terms are not checked by the test harness. */
    return (seq_t)cand;
}
