/*
 * A057516  Number of sea-level valleys in Catalan mountain ranges encoded by
 *          A014486.
 * https://oeis.org/A057516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (number of returns to sea level in the n-th Dyck word of
 *            A014486) - 1, a(0) = 0; found by unranking the n-th Dyck word
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

typedef unsigned __int128 a057516_u128;

static a057516_u128 A057516_W[130][130];
static bool A057516_init = false;

static void A057516_setup(void)
{
    int r, h;
    if (A057516_init) return;
    for (r = 0; r < 130; r++)
        for (h = 0; h < 130; h++)
            A057516_W[r][h] = 0;
    A057516_W[0][0] = 1;
    for (r = 1; r < 130; r++) {
        for (h = 0; h < 130; h++) {
            a057516_u128 v = 0;
            if (h + 1 < 130) v += A057516_W[r - 1][h + 1];
            if (h > 0) v += A057516_W[r - 1][h - 1];
            A057516_W[r][h] = v;
        }
    }
    A057516_init = true;
}

seq_t a(idx_t n)
{
    A057516_setup();
    if (n == 0) return (seq_t)0;
    a057516_u128 m = (a057516_u128)n;
    int k = 0;
    while (true) {
        a057516_u128 c = A057516_W[2 * k][0];
        if (m < c) break;
        m -= c;
        k++;
        if (k > 60) return (seq_t)0;
    }
    if (k == 0) return (seq_t)0;
    int len = 2 * k;
    int h = 0, pos, comps = 0;
    for (pos = 0; pos < len; pos++) {
        int rem = len - pos - 1;
        a057516_u128 cnt = 0;
        if (h > 0) cnt = A057516_W[rem][h - 1];
        if (h > 0 && m < cnt) {
            h--;
        } else {
            m -= cnt;
            h++;
        }
        if (h == 0) comps++;
    }
    return (seq_t)(comps - 1);
}
