/*
 * A171460  Squares in lunar arithmetic in base 4 written in base 4.
 * https://oeis.org/A171460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A171460
   Formula: a(n) is the n-th smallest decimal integer whose decimal
   representation consists only of the digits 0‑3 and which occurs as
   a result of squaring some base‑4 number using lunar arithmetic.
   The values are sorted and deduplicated.  The computation is
   performed once by enumerating all roots with at most 9 base‑4
   digits (262 144 possibilities). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Comparator for qsort */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

/* Static storage for the pre‑computed sequence */
static seq_t  *seq_vals   = NULL;
static size_t  seq_len    = 0;
static bool    prepared   = false;

/* Pre‑compute the sequence up to 9 base‑4 digits (262 144 roots). */
static void precompute(void)
{
    const size_t root_limit = 262144;              /* 4⁹ */
    /* Allocate raw array for all squares (may contain duplicates). */
    seq_t *squares = (seq_t *)malloc(sizeof(seq_t) * root_limit);
    if (!squares) exit(EXIT_FAILURE);

    for (size_t root = 0; root < root_limit; ++root)
    {
        /* Extract base‑4 digits (least significant first). */
        uint8_t digits[16];
        size_t m = 0;
        if (root == 0)
        {
            m = 1;
            digits[0] = 0;
        }
        else
        {
            size_t tmp = root;
            while (tmp)
            {
                digits[m++] = (uint8_t)(tmp % 4);
                tmp /= 4;
            }
        }

        /* Compute lunar square digits: min for each pair, max for summation. */
        uint8_t sq[32] = {0};
        size_t sq_len = 2 * m - 1;
        for (size_t i = 0; i < m; ++i)
            for (size_t j = 0; j < m; ++j)
            {
                uint8_t v = digits[i] < digits[j] ? digits[i] : digits[j];
                if (v > sq[i + j]) sq[i + j] = v;
            }

        /* Convert digit vector to integer: concatenate digits in decimal. */
        seq_t val = 0;
        for (long long i = (long long)sq_len - 1; i >= 0; --i)
            val = val * (seq_t)10 + (seq_t)sq[i];

        squares[root] = val;
    }

    /* Sort all values. */
    qsort(squares, root_limit, sizeof(seq_t), cmp_seq);

    /* Deduplicate. */
    size_t uniq = 0;
    if (root_limit > 0)
    {
        uniq = 1;
        for (size_t i = 1; i < root_limit; ++i)
            if (squares[i] != squares[uniq - 1])
                squares[uniq++] = squares[i];
    }

    /* Allocate final sequence array. */
    seq_vals = (seq_t *)malloc(sizeof(seq_t) * uniq);
    if (!seq_vals) exit(EXIT_FAILURE);
    for (size_t i = 0; i < uniq; ++i) seq_vals[i] = squares[i];
    seq_len = uniq;

    free(squares);
    prepared = true;
}

/* External function required by the harness. */
seq_t a(idx_t n)
{
    if (!prepared) precompute();
    /* The test harness guarantees that n is within the computed range
       (up to at least 10 000). */
    return seq_vals[(size_t)n];
}
