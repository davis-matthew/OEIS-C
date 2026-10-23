/*
 * A179428  Number of ways to place 8 nonattacking kings on an n X n toroidal
 *          board.
 * https://oeis.org/A179428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^14 -252*n^12 +27874*n^10 -1759800*n^8
 *            +68745649*n^6 -1669136028*n^4 +23447322156*n^2
 *            -147931524720)/(40320) for n ≥ 9 For n ≤ 8 the values are
 *            computed by back‑tracking
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 31, u128 n <= 488,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void dfs_brute(int start, int chosen, uint64_t forbidden,
                      const uint64_t *adj, unsigned int total,
                      unsigned long long *cnt)
{
    if (chosen == 8) { ++(*cnt); return; }
    if (total - start < 8 - chosen) return;
    for (int i = start; i < (int)total; ++i) {
        uint64_t bit = 1ULL << i;
        if (forbidden & bit) continue;
        dfs_brute(i + 1, chosen + 1, forbidden | adj[i],
                  adj, total, cnt);
    }
}

static unsigned long long brute_force_kings(uint64_t n)
{
    unsigned int N = (unsigned int)n;
    unsigned int total = N * N;
    uint64_t adj[64] = {0};

    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c) {
            int idx = r * N + c;
            uint64_t mask = 0;
            for (int dr = -1; dr <= 1; ++dr)
                for (int dc = -1; dc <= 1; ++dc) {
                    int nr = (r + dr + N) % N;
                    int nc = (c + dc + N) % N;
                    int nidx = nr * N + nc;
                    mask |= 1ULL << nidx;
                }
            adj[idx] = mask;
        }

    unsigned long long count = 0;
    dfs_brute(0, 0, 0ULL, adj, total, &count);
    return count;
}

seq_t a(idx_t n)
{
    /* Handle small n exactly by enumeration */
    if (n <= 8) {
        return (seq_t)brute_force_kings((uint64_t)n);
    }

    /* Use the polynomial for n ≥ 9 */
    if (sizeof(seq_t) <= 8) {
        unsigned long long nn = (unsigned long long)n;
        unsigned __int128 n2   = (unsigned __int128)nn * nn;   /* n² */
        unsigned __int128 n4   = n2 * n2;                     /* n⁴ */
        unsigned __int128 n6   = n4 * n2;                     /* n⁶ */
        unsigned __int128 n8   = n4 * n4;                     /* n⁸ */
        unsigned __int128 n10  = n6 * n4;                     /* n¹⁰ */
        unsigned __int128 n12  = n6 * n6;                     /* n¹² */
        unsigned __int128 n14  = n12 * n2;                    /* n¹⁴ */

        unsigned __int128 poly =
              n14
            - (unsigned __int128)252      * n12
            + (unsigned __int128)27874    * n10
            - (unsigned __int128)1759800  * n8
            + (unsigned __int128)68745649 * n6
            - (unsigned __int128)1669136028 * n4
            + (unsigned __int128)23447322156 * n2
            - (unsigned __int128)147931524720;

        unsigned __int128 res = n2 * poly / (unsigned __int128)40320;
        return (seq_t)res;
    } else {
        seq_t sn   = (seq_t)n;
        seq_t n2   = sn * sn;          /* n² */
        seq_t n4   = n2 * n2;          /* n⁴ */
        seq_t n6   = n4 * n2;          /* n⁶ */
        seq_t n8   = n4 * n4;          /* n⁸ */
        seq_t n10  = n6 * n4;          /* n¹⁰ */
        seq_t n12  = n6 * n6;          /* n¹² */
        seq_t n14  = n12 * n2;         /* n¹⁴ */

        seq_t poly =
              n14
            - (seq_t)252      * n12
            + (seq_t)27874    * n10
            - (seq_t)1759800  * n8
            + (seq_t)68745649 * n6
            - (seq_t)1669136028 * n4
            + (seq_t)23447322156 * n2
            - (seq_t)147931524720;

        seq_t res = n2 * poly / (seq_t)40320;
        return res;
    }
}
