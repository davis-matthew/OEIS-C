/*
 * A230998  Number of years after which it is not possible to have a date
 *          falling on the same day of the week, in the Gregorian calendar.
 * https://oeis.org/A230998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th positive integer k for which there is NO *
 *            starting year Y (1 ≤ Y ≤ 400) such that k + L(Y,k) ≡ 0 (mod 7),
 *            * where L(Y,k) is the number of leap years in the k‐year
 *            interval * beginning with year Y. This set is periodic with
 *            period 400, so the * nth term can be obtained by interle...
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static bool initialized = false;
static size_t t = 0;                 /* number of residues in the set */
static unsigned int residues[400];   /* sorted residues < 400 for which a(k) is NOT possible */

static void precompute(void)
{
    /* Precompute leap year flags and prefix sums for years 1..400 */
    unsigned char isLeap[401] = {0};
    for (int y = 1; y <= 400; ++y) {
        if ((y % 4 == 0) && ((y % 100 != 0) || (y % 400 == 0)))
            isLeap[y] = 1;
    }
    int pref[401];
    pref[0] = 0;
    for (int i = 1; i <= 400; ++i)
        pref[i] = pref[i - 1] + isLeap[i];

    /* Determine which k (1..400) are possible */
    bool possible[401] = {0};
    for (int k = 1; k <= 400; ++k) {
        bool can = false;
        int k_mod7 = k % 7;
        for (int y = 1; y <= 400 && !can; ++y) {
            int start = y;
            int end   = y + k - 1;
            int leaps;
            if (end <= 400) {
                leaps = pref[end] - pref[start - 1];
            } else {
                int rem = end - 400;                     /* 1 <= rem <= 399 */
                leaps = (pref[400] - pref[start - 1]) + pref[rem];
            }
            leaps %= 7;
            if ((k_mod7 + leaps) % 7 == 0)
                can = true;
        }
        possible[k] = can;
    }

    /* Collect residues where possible[k] == false */
    for (int k = 1; k <= 400; ++k) {
        if (!possible[k]) {
            residues[t++] = (unsigned int)k;
        }
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        precompute();
        initialized = true;
    }

    /* The sequence is t numbers per 400‑year cycle */
    idx_t zero = n - 1;
    idx_t cycle = zero / t;          /* number of full cycles before this term */
    idx_t pos   = zero % t;          /* position within the current cycle */

    seq_t val = (seq_t)residues[pos] + (seq_t)400 * (seq_t)cycle;
    return val;
}
