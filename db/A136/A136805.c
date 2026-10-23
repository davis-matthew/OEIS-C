/*
 * A136805  Squares mod 65537.
 * https://oeis.org/A136805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, easy, nonn
 * Formula:   a(n) is the nth smallest quadratic residue modulo 65537 (i.e.,
 *            the nth element in the sorted list of {k^2 mod 65537 | 0 ≤ k <
 *            65537})
 * Range:     u8 n <= 125, u16 n <= 9613, u32 n <= 9613, u64 n <= 9613,
 *            u128 n <= 9613, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

static seq_t *squares = NULL;
static size_t squares_len = 0;
static bool initialized = false;

static void init_squares(void)
{
    /* Use a local boolean array to mark which residues occur. */
    static bool seen[65537] = {0};

    idx_t i;
    for (i = 0; i < 65537; ++i) {
        /* i * i fits in idx_t (which is at least 64 bits). */
        idx_t sq = (i * i) % 65537;
        seen[sq] = true;
    }

    /* Count distinct residues (including 0). */
    size_t count = 0;
    for (i = 0; i < 65537; ++i)
        if (seen[i])
            ++count;

    squares_len = count;
    squares = (seq_t *)malloc(squares_len * sizeof(seq_t));

    /* Fill the array with sorted residues. */
    size_t pos = 0;
    for (i = 0; i < 65537; ++i) {
        if (seen[i]) {
            squares[pos++] = (seq_t)i;  /* cast to seq_t (safe because i ≤ 65536) */
        }
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_squares();
        initialized = true;
    }
    if (n == 0 || n > squares_len)
        return (seq_t)0;
    return squares[n - 1];
}
