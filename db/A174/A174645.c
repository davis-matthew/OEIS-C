/*
 * A174645  Number of ways to place 6 nonattacking amazons (superqueens) on a
 *          6 X n board.
 * https://oeis.org/A174645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * For n >= 17 the sequence is given by the degree‑6 polynomial *
 *            * a(n) = n^6 - 63*n^5 + 1775*n^4 - 28613*n^3 * + 277462*n^2 -
 *            1526716*n + 3699966 * * For 1 <= n < 17 the value is computed by
 *            exhaustive enumeration * of all 6‑column selections on a 6×n
 *            board and all ways to assign * the 6 rows to...
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 50, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 52 terms through n = 949 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Helper: recursive enumeration of row permutations for a fixed column set.
 * cols[] contains the 6 selected column indices (sorted ascending).
 * pos  = current depth (0..6).
 * used = bit mask of rows already used.
 * perm = array storing the current assignment of rows to columns.
 *
 * Returns the number of valid full permutations that extend the current prefix.
 */
static seq_t recurse_perm(const idx_t cols[6], int pos, int used, int perm[])
{
    if (pos == 6) {
        /* All six pieces placed without conflict. */
        return 1;
    }

    seq_t count = 0;
    for (int r = 0; r < 6; ++r) {
        if (!(used & (1 << r))) {
            int ok = 1;
            /* Check conflicts with already placed pieces. */
            for (int k = 0; k < pos; ++k) {
                int dr = (r > perm[k]) ? (r - perm[k]) : (perm[k] - r);
                idx_t dc = (cols[pos] > cols[k]) ? (cols[pos] - cols[k]) : (cols[k] - cols[pos]);

                if (dr == dc) {          /* same diagonal */
                    ok = 0; break;
                }
                if ((dc == 1 && dr == 2) || (dc == 2 && dr == 1)) { /* knight move */
                    ok = 0; break;
                }
            }
            if (ok) {
                perm[pos] = r;
                count += recurse_perm(cols, pos + 1, used | (1 << r), perm);
            }
        }
    }
    return count;
}

/* Exhaustive enumeration for n < 17. */
static seq_t brute_small(idx_t n)
{
    if (n < 6) {
        return 0;  /* cannot place 6 non‑attacking amazons */
    }

    seq_t total = 0;
    int perm[6];

    /* Enumerate all 6‑column selections in increasing order. */
    for (idx_t a = 0; a + 5 < n; ++a)
    for (idx_t b = a + 1; b + 4 < n; ++b)
    for (idx_t c = b + 1; c + 3 < n; ++c)
    for (idx_t d = c + 1; d + 2 < n; ++d)
    for (idx_t e = d + 1; e + 1 < n; ++e)
    for (idx_t f = e + 1; f < n; ++f)
    {
        idx_t cols[6] = {a, b, c, d, e, f};
        total += recurse_perm(cols, 0, 0, perm);
    }

    return total;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n >= 17) {
        /* Compute the polynomial using Horner's rule. */
        seq_t n_seq   = (seq_t)n;
        seq_t n_sq    = n_seq * n_seq;          /* n^2  */
        seq_t n_cb    = n_sq * n_seq;           /* n^3  */
        seq_t n_4     = n_cb * n_seq;           /* n^4  */
        seq_t n_5     = n_4  * n_seq;           /* n^5  */
        seq_t n_6     = n_5  * n_seq;           /* n^6  */

        seq_t res = n_6;
        res -= (seq_t)63 * n_5;
        res += (seq_t)1775 * n_4;
        res -= (seq_t)28613 * n_cb;
        res += (seq_t)277462 * n_sq;
        res -= (seq_t)1526716 * n_seq;
        res += (seq_t)3699966;
        return res;
    } else {
        return brute_small(n);
    }
}
