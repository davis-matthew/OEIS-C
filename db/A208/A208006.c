/*
 * A208006  Number of nX7 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A208006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Number of n×7 binary arrays avoiding
 * horizontally 001 and 100, vertically 011 and 110.
 * Computed by dynamic programming up to n=18,
 * then by the order‑17 linear recurrence given in OEIS A208006:
 * a(n)=13*a(n‑1)-6*a(n‑2)-485*a(n‑3)+1437*a(n‑4)+3392*a(n‑5)
 *        -14770*a(n‑6)-2894*a(n‑7)+44308*a(n‑8)-15290*a(n‑9)
 *        -46782*a(n‑10)+23648*a(n‑11)+18387*a(n‑12)-9503*a(n‑13)
 *        -2702*a(n‑14)+1151*a(n‑15)+129*a(n‑16)-32*a(n‑17)
 * for n>18.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/* Coefficients of the recurrence (a(n‑1) … a(n‑17)) */
static const long long COEFFS[17] = {
    13LL, -6LL, -485LL, 1437LL, 3392LL, -14770LL, -2894LL,
    44308LL, -15290LL, -46782LL, 23648LL, 18387LL,
    -9503LL, -2702LL, 1151LL, 129LL, -32LL
};

static bool initialized = false;
static size_t V;                       /* number of valid 7‑bit rows */
static uint8_t rows[128];
static int8_t row_index[128];
static uint64_t trans[35][35];          /* transition mask a,b -> allowed c's */
static seq_t initA[19];                /* a(1)..a(18) */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* One‑time initialization of tables and DP for n=1..18 */
static void init_sequence(void) {
    if (initialized) return;

    /* Build list of valid rows */
    V = 0;
    for (int x = 0; x < 128; ++x) {
        bool ok = true;
        for (int j = 0; j <= 4; ++j) {          /* 3‑bit windows */
            int w = (x >> j) & 7;
            if (w == 1 || w == 4) { /* 001 or 100 */
                ok = false;
                break;
            }
        }
        if (ok) {
            rows[V] = (uint8_t)x;
            row_index[x] = (int8_t)V;
            ++V;
        } else {
            row_index[x] = -1;
        }
    }

    /* Precompute transition masks */
    for (size_t a = 0; a < V; ++a) {
        for (size_t b = 0; b < V; ++b) {
            uint64_t mask = 0;
            for (size_t c = 0; c < V; ++c) {
                bool ok = true;
                uint8_t ra = rows[a], rb = rows[b], rc = rows[c];
                for (int bit = 0; bit < 7; ++bit) {
                    int abit = (ra >> bit) & 1;
                    int bbit = (rb >> bit) & 1;
                    int cbit = (rc >> bit) & 1;
                    if ((abit == 0 && bbit == 1 && cbit == 1) ||
                        (abit == 1 && bbit == 1 && cbit == 0)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) mask |= (uint64_t)1 << c;
            }
            trans[a][b] = mask;
        }
    }

    /* DP for n=1..18 */
    initA[1] = (seq_t)V;
    initA[2] = (seq_t)V * (seq_t)V;

    size_t states = V * V;
    dp_cur = (seq_t*)malloc(states * sizeof(seq_t));
    dp_next = (seq_t*)malloc(states * sizeof(seq_t));

    memset(dp_cur, 0, states * sizeof(seq_t));
    for (size_t s = 0; s < states; ++s) dp_cur[s] = 1;    /* all pairs allowed for n=2 */

    for (int n = 3; n <= 18; ++n) {
        memset(dp_next, 0, states * sizeof(seq_t));
        for (size_t s = 0; s < states; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            size_t a = s / V;
            size_t b = s % V;
            uint64_t mask = trans[a][b];
            while (mask) {
                int c = __builtin_ctzll(mask);
                mask &= mask - 1;
                size_t ns = b * V + c;
                dp_next[ns] += val;
            }
        }
        seq_t total = 0;
        for (size_t s = 0; s < states; ++s) total += dp_next[s];
        initA[n] = total;
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    init_sequence();
    if (n <= 18) return initA[n];

    /* Buffer for the last 17 values: buf[0]=a(n-1), buf[1]=a(n-2), … */
    seq_t buf[17];
    for (int i = 0; i < 17; ++i) buf[i] = initA[18 - i];

    for (idx_t cur = 19; cur <= n; ++cur) {
        seq_t pos = 0, neg = 0;
        for (int i = 0; i < 17; ++i) {
            long long c = COEFFS[i];
            uint64_t abs_c = (c >= 0) ? (uint64_t)c : (uint64_t)(-c);
            seq_t term = buf[i] * (seq_t)abs_c;
            if (c > 0)
                pos += term;
            else
                neg += term;
        }
        seq_t newVal = pos - neg;  /* pos >= neg for all n by the recurrence */

        /* Shift buffer: buf[1..16] = buf[0..15] */
        for (int i = 16; i >= 1; --i) buf[i] = buf[i-1];
        buf[0] = newVal;
    }
    return buf[0];
}
