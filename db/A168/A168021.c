/*
 * A168021  Triangle T(n,k) read by rows in which row n lists the number of
 *          partitions of n into parts divisible by k.
 * https://oeis.org/A168021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,c) where T(r,k) = partition(r/k) if k divides r else
 *            0; The sequence is read by rows of the triangle T(n,k);
 *            partitions are computed by the standard DP (p[0]=1; p[i]+=p[i-j]
 *            for j<=i)
 * Range:     u8 n <= 136, u16 n <= 946, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Adjust this limit if larger indices may be requested. */
#define MAX_PART 5000

static seq_t parts[MAX_PART + 1];
static bool parts_computed = false;

/* Compute partition numbers up to MAX_PART using the DP algorithm. */
static void compute_partitions(void)
{
    parts[0] = 1;
    for (idx_t i = 1; i <= MAX_PART; ++i)
        parts[i] = 0;

    for (idx_t i = 1; i <= MAX_PART; ++i) {
        for (idx_t j = i; j <= MAX_PART; ++j) {
            parts[j] += parts[j - i];
        }
    }
    parts_computed = true;
}

/* Return the n‑th term of A168021. */
seq_t a(idx_t n)
{
    if (!parts_computed)
        compute_partitions();

    /* Find the row r such that triangular(r-1) < n <= triangular(r). */
    idx_t r = 1;
    idx_t triangular = 1;                /* triangular(r) = r*(r+1)/2 */
    while (triangular < n) {
        ++r;
        triangular = r * (r + 1) / 2;
    }

    /* Column within the row. */
    idx_t prev_triangular = (r - 1) * r / 2;
    idx_t col = n - prev_triangular;     /* 1 <= col <= r */

    /* If col does not divide r, the entry is 0. */
    if (r % col != 0)
        return 0;

    idx_t m = r / col;                   /* m <= r <= MAX_PART always holds. */
    return parts[m];
}
