/*
 * A173070  Palindromic mountain numbers.
 * https://oeis.org/A173070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the n-th palindromic mountain number. A palindromic
 *            mountain number is a decimal palindrome whose digits strictly
 *            increase to the middle digit and then strictly decrease
 *            symmetrically, with the first digit equal to 1. The numbers are
 *            enumerated in ascending order. For n ≥ 1 the function re...
 * Range:     u8 n <= 9, u16 n <= 37, u32 n <= 163, u64 n <= 256,
 *            u128 n <= 256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* Undefined for n < 1, return 0 as safe value */

    /* Precompute binomial coefficients C[0..8][0..8] */
    static idx_t C[9][9];
    static int Cdone = 0;
    if (!Cdone) {
        for (int i = 0; i <= 8; ++i) {
            C[i][0] = 1;
            C[i][i] = 1;
            for (int j = 1; j < i; ++j)
                C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
        }
        Cdone = 1;
    }

    idx_t rank = n - 1;            /* 0‑based lexicographic rank of the subset */
    int     k = 0;                 /* size of the subset (number of chosen digits) */

    /* Find subset size k such that
       sum_{i=0}^{k-1} C[8][i] <= rank < sum_{i=0}^{k} C[8][i] */
    for (int s = 0; s <= 8; ++s) {
        idx_t cnt = C[8][s];
        if (rank < cnt) {
            k = s;
            break;
        }
        rank -= cnt;
    }

    /* If k == 0, the number is simply 1 */
    if (k == 0)
        return 1;

    /* Decode the rank into the actual combination indices.
       The indices are in [0..7] corresponding to digits 2..9. */
    int idx[8];                   /* indices of chosen digits */
    int pos = 0;                  /* number of indices selected so far */
    idx_t r = rank;               /* remaining rank within size k combinations */

    for (int i = 0; i <= 7 && pos < k; ++i) {
        int avail = 8 - i;                 /* elements remaining including i */
        if (avail < k - pos) continue;     /* not enough elements left */
        idx_t ways = C[avail - 1][k - pos - 1];
        if (r < ways) {
            idx[pos++] = i;
        } else {
            r -= ways;
        }
    }

    /* Build the number from most significant digit to least.
       Digits: 1, (2+idx[0]), (2+idx[1]), …, (2+idx[k-1]), mirrored (excluding peak). */
    seq_t result = 1;
    for (int i = 0; i < k; ++i)
        result = result * 10 + (seq_t)(2 + idx[i]);

    /* Append the mirrored digits */
    for (int i = k - 1; i >= 0; --i) {
        seq_t d = (i == 0) ? 1 : (seq_t)(2 + idx[i - 1]);
        result = result * 10 + d;
    }

    return result;
}
