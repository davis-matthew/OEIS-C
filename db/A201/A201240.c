/*
 * A201240  Number of ways to place 6 non-attacking wazirs on an n X n
 *          toroidal board.
 * https://oeis.org/A201240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n^2*(n^10 - 75*n^8 + 2365*n^6 - 39285*n^4 + 345034*n^2 - 1288680)/720, n>=7
 */
seq_t a(idx_t n)
{
    /* small values handled explicitly */
    switch (n) {
        case 1: return 0;
        case 2: return 0;
        case 3: return 0;
        case 4: return 56ULL;
        case 5: return 7100ULL;
        case 6: return 252792ULL;
    }

    /* compute via the closed‑form polynomial */
    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;
    seq_t n4   = n2 * n2;
    seq_t n6   = n4 * n2;
    seq_t n8   = n4 * n4;
    seq_t n10  = n8 * n2;

    seq_t poly = n10
                 - (seq_t)75   * n8
                 + (seq_t)2365  * n6
                 - (seq_t)39285 * n4
                 + (seq_t)345034 * n2
                 - (seq_t)1288680;

    seq_t numerator = n2 * poly;
    seq_t result    = numerator / 720;
    return result;
}
