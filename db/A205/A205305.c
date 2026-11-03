/*
 * A205305  Number of nX3 0..3 arrays with no occurrence of three equal
 *          elements in a row horizontally or vertically, and new values 0..3
 *          introduced in row major order.
 * https://oeis.org/A205305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 200 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 45*a(n‑1) + 360*a(n‑2) - 2443*a(n‑3) + 752*a(n‑4)
 *           + 1861*a(n‑5) + 17330*a(n‑6) + 3931*a(n‑7)
 *           - 55696*a(n‑8) - 6621*a(n‑9) + 28764*a(n‑10)
 *           - 5265*a(n‑11) - 9477*a(n‑12) + 1080*a(n‑13) + 1080*a(n‑14)
 *
 * Initial values:
 *   a(1)=4, a(2)=159, a(3)=7445, a(4)=381958,
 *   a(5)=19490790, a(6)=996536552, a(7)=50933680120,
 *   a(8)=2603456557653, a(9)=133072628448179,
 *   a(10)=6801873967993408, a(11)=347670725973943044,
 *   a(12)=17770830778194675356, a(13)=908337669425005770376,
 *   a(14)=46428742540607711341143
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

static const long long coef[14] = {
    45LL, 360LL, -2443LL, 752LL, 1861LL,
    17330LL, 3931LL, -55696LL, -6621LL,
    28764LL, -5265LL, -9477LL, 1080LL,
    1080LL
};

static const char * const init_strs[14] = {
    "4", "159", "7445", "381958", "19490790",
    "996536552", "50933680120", "2603456557653",
    "133072628448179", "6801873967993408",
    "347670725973943044", "17770830778194675356",
    "908337669425005770376", "46428742540607711341143"
};

static seq_t init_vals[14];
static bool init_done = false;

/* Initialise the table of exact initial values. */
static void init_table(void)
{
    if (!init_done) {
        for (int i = 0; i < 14; ++i) {
            init_vals[i] = seq_from_str(init_strs[i]);
        }
        init_done = true;
    }
}

seq_t a(idx_t n)
{
    init_table();

    if (n == 0) return init_vals[0] - init_vals[0];      /* unspecified, but safe */

    if (n <= 14) return init_vals[n - 1];

    /* Ring buffer containing a(i-1), a(i-2), …, a(i-14) */
    seq_t ring[14];
    for (int j = 0; j < 14; ++j) {
        ring[j] = init_vals[13 - j];
    }

    /* Compute a(15) … a(n) */
    for (idx_t i = 15; i <= n; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (int k = 0; k < 14; ++k) {
            long long c = coef[k];
            seq_t prod = mul_small(ring[k], (unsigned)((c >= 0) ? c : -c));
            if (c >= 0) {
                sum_pos += prod;
            } else {
                sum_neg += prod;
            }
        }
        seq_t next = sum_pos;
        next -= sum_neg;          /* exact difference, may be negative temporarily */

        /* shift ring: drop oldest, insert new at front */
        for (int j = 13; j > 0; --j) {
            ring[j] = ring[j - 1];
        }
        ring[0] = next;
    }

    return ring[0];
}
