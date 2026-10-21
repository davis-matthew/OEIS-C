/*
 * A235318  Number of (n+1) X (7+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 7
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 14, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A235318: number of (n+1)×8 arrays of digits 0..6 whose every
    2×2 subblock has diagonal sum differing from antidiagonal sum by 7.
    The enumeration can be reduced to a transition between the
    difference vectors of consecutive rows.  
    For a row r the vector
        d[i] = a_r[i] – a_r[i+1]          (  i = 0..6 )
    must have all entries non‑zero (otherwise no neighbour row exists).
    If a row has difference vector d, its neighbouring row must have
    the vector f(d) where each component changes sign and is shifted
    by 7:
        f(d)[i] = d[i] - 7   if d[i] > 0
        f(d)[i] = d[i] + 7   if d[i] < 0
    For a given difference vector d the number of admissible rows is
    the number of possible first entries a0 such that all 8 entries
    stay in 0..6.  This equals
        val(d) = max(0, min_i(s_i) - max_i(s_i) + 7)
    where s_i are the partial sums of the differences.
    The total number of arrays with (n+1) rows is then
        a(n) = Σ_{d∈D} val(d)^{ceil((n+1)/2)} · val(f(d))^{floor((n+1)/2)},
    summing over all 7‑tuples d of non‑zero values from –6..–1,1..6.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t power_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /*  Precompute the frequency table of (val(d), val(f(d))) pairs.
        The table contains at most 49 non‑zero entries.  The table is
        built once on the first call.  It is valid for all subsequent
        calls regardless of the order of the n values.  */
    static bool initialized = false;
    static uint64_t cnt[8][8] = {0};

    if (!initialized) {
        const int diff_lookup[12] = { -6,-5,-4,-3,-2,-1, 1,2,3,4,5,6 };
        const uint32_t LIMIT = 35831808U;          /* 12^7  */
        uint8_t diff[7] = {0};

        for (uint32_t iter = 0; iter < LIMIT; ++iter) {
            int S = 0, maxS = 0, minS = 0;
            int G = 0, maxSp = 0, minSp = 0;

            for (int pos = 0; pos < 7; ++pos) {
                int d = diff_lookup[diff[pos]];
                S += d;
                if (S > maxS) maxS = S;
                if (S < minS) minS = S;
                int sign = (d > 0) ? 1 : -1;
                G += sign;
                int Sp = S - 7 * G;
                if (Sp > maxSp) maxSp = Sp;
                if (Sp < minSp) minSp = Sp;
            }

            int val_d = minS - maxS + 7;
            if (val_d < 0) val_d = 0;
            int val_f = minSp - maxSp + 7;
            if (val_f < 0) val_f = 0;

            if (val_d > 0 && val_f > 0) {
                ++cnt[val_d][val_f];
            }

            /*  increment the base‑12 counter  */
            for (int pos = 0; pos < 7; ++pos) {
                ++diff[pos];
                if (diff[pos] < 12) break;
                diff[pos] = 0;
            }
        }
        initialized = true;
    }

    /*  Compute a(n) using the precomputed table.  */
    idx_t rows   = n + 1;
    idx_t expA   = (rows + 1) / 2;   /* ceil(rows/2)   */
    idx_t expB   = rows / 2;         /* floor(rows/2)  */

    seq_t total = 0;

    for (int i = 1; i <= 7; ++i) {
        seq_t pow_i = power_seq((seq_t)i, expA);
        for (int j = 1; j <= 7; ++j) {
            uint64_t c = cnt[i][j];
            if (!c) continue;
            seq_t pow_j    = power_seq((seq_t)j, expB);
            seq_t term     = (seq_t)c;
            term *= pow_i;
            term *= pow_j;
            total += term;
        }
    }
    return total;
}
