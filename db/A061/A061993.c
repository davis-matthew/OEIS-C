/*
 * A061993  Number of ways to place 7 nonattacking queens on a 7 X n board.
 * https://oeis.org/A061993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 31, u64 n <= 574,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A061993: number of ways to place 7 non‑attacking queens on a 7×n board
 *  Formula (V. Kotesovec, 1992) valid for n ≥ 23:
 *      a(n) = n^7 - 63*n^6 + 1879*n^5 - 34411*n^4 + 417178*n^3
 *             - 3336014*n^2 + 16209916*n - 36693996
 *  For n < 23 the values are given explicitly (first non‑zero entry starts at n = 7).
 *  The function is fully deterministic and does not use any pre‑computed
 *  dynamic tables beyond the few constant values needed for n < 23.
 */

seq_t a(idx_t n)
{
    /* pre‑computed terms for 0 ≤ n < 23 (from the OEIS entry) */
    static const seq_t base[24] = {
        /* 0 1 2 3 4 5 6 7  8   9    10    11     12      13      14      15      16      17      18      19      20      21      22      23  */
          0, 0, 0, 0, 0, 0, 0, 40, 312, 2038, 9632, 37248, 120104, 335010, 835056,
        1897702, 3998456, 7907094, 14818300, 26512942, 45562852, 75580634, 121520020, 190031678
    };

    if (n < 24)
        return base[n];

    /* Compute the explicit polynomial for n ≥ 23 */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          // n^2
    seq_t n3 = n2 * nn;          // n^3
    seq_t n4 = n3 * nn;          // n^4
    seq_t n5 = n4 * nn;          // n^5
    seq_t n6 = n5 * nn;          // n^6
    seq_t n7 = n6 * nn;          // n^7

    seq_t result =
          n7
        - (seq_t)63 * n6
        + (seq_t)1879 * n5
        - (seq_t)34411 * n4
        + (seq_t)417178 * n3
        - (seq_t)3336014 * n2
        + (seq_t)16209916 * nn
        - (seq_t)36693996;
    return result;
}
