/*
 * A186475  Number of (n+1) X 3 0..3 arrays with every 2 X 2 subblock
 *          commuting with each of its horizontal and vertical 2 X 2 subblock
 *          neighbors.
 * https://oeis.org/A186475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+2*a(n-2)-6*a(n-3)+6*a(n-4)-3*a(n-5) for n>13
 *            (initial values a(1) … a(13) are computed by dynamic
 *            programming)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 24, u64 n <= 54, u128 n <= 114,
 *            big any n
 * Helpers:   get_cell
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/get_cell.h"

static unsigned char H[64][64];               /* horizontal compatibility */
static unsigned char V[64][64][64];           /* vertical compatibility */
static unsigned char trans_list[64][64];     /* lists of transitions */
static unsigned char trans_len[64];
static int tables_initialized = 0;

/* check if two 2×2 matrices commute */
static inline int commute(unsigned int a00, unsigned int a01,
                          unsigned int a10, unsigned int a11,
                          unsigned int b00, unsigned int b01,
                          unsigned int b10, unsigned int b11)
{
    unsigned int m00 = a00 * b00 + a01 * b10;
    unsigned int m01 = a00 * b01 + a01 * b11;
    unsigned int m10 = a10 * b00 + a11 * b10;
    unsigned int m11 = a10 * b01 + a11 * b11;

    unsigned int n00 = b00 * a00 + b01 * a10;
    unsigned int n01 = b00 * a01 + b01 * a11;
    unsigned int n10 = b10 * a00 + b11 * a10;
    unsigned int n11 = b10 * a01 + b11 * a11;

    return (m00 == n00) && (m01 == n01) && (m10 == n10) && (m11 == n11);
}

/* build static compatibility tables once */
static void build_tables(void)
{
    for (int a = 0; a < 64; ++a) {
        unsigned int a0 = get_cell(a, 0), a1 = get_cell(a, 1), a2 = get_cell(a, 2);
        for (int b = 0; b < 64; ++b) {
            unsigned int b0 = get_cell(b, 0), b1 = get_cell(b, 1), b2 = get_cell(b, 2);
            /* horizontal: M0(a,b) commutes with M1(a,b) */
            H[a][b] = (unsigned char)
                (commute(a0, a1, b0, b1,
                         a1, a2, b1, b2) ? 1 : 0);
        }
    }
    for (int a = 0; a < 64; ++a) {
        unsigned int a0 = get_cell(a, 0), a1 = get_cell(a, 1), a2 = get_cell(a, 2);
        for (int b = 0; b < 64; ++b) {
            unsigned int b0 = get_cell(b, 0), b1 = get_cell(b, 1), b2 = get_cell(b, 2);
            for (int c = 0; c < 64; ++c) {
                unsigned int c0 = get_cell(c, 0), c1 = get_cell(c, 1), c2 = get_cell(c, 2);
                int ok0 = commute(a0, a1, b0, b1,
                                  b0, b1, c0, c1);
                int ok1 = commute(a1, a2, b1, b2,
                                  b1, b2, c1, c2);
                V[a][b][c] = (unsigned char)((ok0 && ok1) ? 1 : 0);
            }
        }
    }
    for (int b = 0; b < 64; ++b) {
        unsigned char len = 0;
        for (int c = 0; c < 64; ++c) {
            if (H[b][c]) trans_list[b][len++] = (unsigned char)c;
        }
        trans_len[b] = len;
    }
    tables_initialized = 1;
}

/* compute the first 13 terms once */
static seq_t init_vals[14];   /* init_vals[1] … init_vals[13] */
static int init_computed = 0;

static void compute_initial_terms(void)
{
    if (!tables_initialized) build_tables();

    const int MAX_N = 13;          /* we will generate a(1)…a(13) */
    const int STATES = 64 * 64;

    seq_t dp[STATES];
    seq_t newdp[STATES];

    /* clear arrays */
    for (int i = 0; i < STATES; ++i) dp[i] = (seq_t)0;
    for (int i = 0; i < STATES; ++i) newdp[i] = (seq_t)0;

    /* initial pair of rows: a(1) */
    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            dp[a * 64 + b] = (seq_t)H[a][b];
        }
    }

    /* sum for n = 1 */
    seq_t sum = 0;
    for (int i = 0; i < STATES; ++i) sum += dp[i];
    init_vals[1] = sum;

    /* iterative DP for n = 2 … 13 */
    for (int step = 3; step <= MAX_N + 1; ++step) {
        /* zero newdp */
        for (int i = 0; i < STATES; ++i) newdp[i] = (seq_t)0;
        for (int a = 0; a < 64; ++a) {
            for (int b = 0; b < 64; ++b) {
                seq_t val = dp[a * 64 + b];
                if (!val) continue;
                unsigned char len = trans_len[b];
                unsigned char *list = trans_list[b];
                for (unsigned char ki = 0; ki < len; ++ki) {
                    int c = list[ki];
                    if (V[a][b][c]) {
                        newdp[b * 64 + c] += val;
                    }
                }
            }
        }
        /* copy newdp to dp */
        for (int i = 0; i < STATES; ++i) dp[i] = newdp[i];
        /* compute sum: this is a(step-1) */
        seq_t s = 0;
        for (int i = 0; i < STATES; ++i) s += dp[i];
        init_vals[step - 1] = s;
    }

    init_computed = 1;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n<1 */

    if (!init_computed) compute_initial_terms();

    if (n <= 13) return init_vals[n];

    /* recurrence for n >= 14 using last five terms of a(9) … a(13) */
    seq_t t1 = init_vals[9];   /* a(n-5) */
    seq_t t2 = init_vals[10];  /* a(n-4) */
    seq_t t3 = init_vals[11];  /* a(n-3) */
    seq_t t4 = init_vals[12];  /* a(n-2) */
    seq_t t5 = init_vals[13];  /* a(n-1) */

    for (idx_t k = 14; k <= n; ++k) {
        seq_t sum_pos = t5 * (seq_t)2u + t4 * (seq_t)2u + t2 * (seq_t)6u;
        seq_t sum_neg = t3 * (seq_t)6u + t1 * (seq_t)3u;
        seq_t next = sum_pos - sum_neg;

        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = t5;
        t5 = next;
    }
    return t5;
}
