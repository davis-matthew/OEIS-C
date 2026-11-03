/*
 * A194788  Number of ways to place 7 nonattacking kings on an n X n board.
 * https://oeis.org/A194788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^14 - 189n^12 + 252n^11 + 15211n^10 - 38640n^9 -
 *            649215n^8 + 2408700n^7 + 14771764n^6 - 75856200n^5 -
 *            144099396n^4 + 1198867488n^3 - 255900576n^2 - 7543005120n +
 *            10617929280)/5040 for n >= 6; a(1)=a(2)=a(3)=a(4)=0, a(5)=242
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 6, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive helper for the 5×5 board DFS */
static void dfs5(int pos, int placed, uint32_t blocked,
                 uint32_t *attack, unsigned long long *total)
{
    if (placed == 7) {
        ++(*total);
        return;
    }
    if (pos >= 25) return;

    for (int i = pos; i < 25; ++i) {
        if (!(blocked & (1u << i))) {
            dfs5(i + 1, placed + 1, blocked | attack[i], attack, total);
        }
    }
}

/* Count the 7‑king arrangements on a 5×5 board (unenforced time). */
static seq_t count_kings_5(void)
{
    uint32_t attack[25];
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 5; ++c) {
            int idx = r * 5 + c;
            uint32_t mask = 0;
            for (int dr = -1; dr <= 1; ++dr) {
                int rr = r + dr;
                if (rr < 0 || rr >= 5) continue;
                for (int dc = -1; dc <= 1; ++dc) {
                    int cc = c + dc;
                    if (cc < 0 || cc >= 5) continue;
                    int p = rr * 5 + cc;
                    mask |= (1u << p);
                }
            }
            attack[idx] = mask;
        }
    }

    unsigned long long total = 0;
    dfs5(0, 0, 0, attack, &total);
    return (seq_t)total;
}

/* forward declaration for the DFS helper used to count the 5×5 board */
static void dfs5(int pos, int placed, uint32_t blocked,
                 uint32_t *attack, unsigned long long *total);

/* Core sequence function */
seq_t a(idx_t n)
{
    if (n < 5) return 0;           /* a(1)=a(2)=a(3)=a(4)=0          */
    if (n == 5) return count_kings_5(); /* a(5)=242 via brute force     */

    /* n >= 6 : evaluate the polynomial. All arithmetic is with seq_t. */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;            /* n^2 */
    seq_t n3 = n2 * sn;            /* n^3 */
    seq_t n4 = n2 * n2;            /* n^4 */
    seq_t n5 = n4 * sn;            /* n^5 */
    seq_t n6 = n5 * sn;            /* n^6 */
    seq_t n7 = n6 * sn;            /* n^7 */
    seq_t n8 = n4 * n4;            /* n^8 */
    seq_t n9 = n8 * sn;            /* n^9 */
    seq_t n10 = n5 * n5;           /* n^10 */
    seq_t n11 = n10 * sn;          /* n^11 */
    seq_t n12 = n6 * n6;           /* n^12 */
    seq_t n13 = n12 * sn;          /* n^13 */
    seq_t n14 = n7 * n7;           /* n^14 */

    seq_t pos = n14
              + (seq_t)252 * n11
              + (seq_t)15211 * n10
              + (seq_t)2408700 * n7
              + (seq_t)14771764 * n6
              + (seq_t)1198867488 * n3
              + (seq_t)10617929280;

    seq_t neg = (seq_t)189 * n12
              + (seq_t)38640 * n9
              + (seq_t)649215 * n8
              + (seq_t)75856200 * n5
              + (seq_t)144099396 * n4
              + (seq_t)255900576 * n2
              + (seq_t)7543005120 * sn;

    seq_t numerator = pos - neg;
    return numerator / (seq_t)5040;
}
