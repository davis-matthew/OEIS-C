/*
 * A237457  Number of (n+1)X(2+1) 0..2 arrays with the maximum plus the lower
 *          median of every 2X2 subblock differing from its horizontal and
 *          vertical neighbors by exactly one.
 * https://oeis.org/A237457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is computed by dynamic programming on row
 *   configurations for a (n+1)×3 0..2 array.
 *   For 1 ≤ n ≤ 19 the values are obtained by that DP.
 *   For n > 19 the sequence satisfies the (empirical) linear recurrence
 *
 *   a(n) = 34·a(n-3) + 172·a(n-4) + 254·a(n-5) + 282·a(n-6)
 *          - 973·a(n-7) - 4066·a(n-8) - 3238·a(n-9) + 3634·a(n-10)
 *          +13908·a(n-11) + 5602·a(n-12) -22988·a(n-13) -3264·a(n-14)
 *          +22488·a(n-15) - 7392·a(n-16) -13696·a(n-17) + 4608·a(n-18)
 *          + 4096·a(n-19) ,
 *
 *   where the terms are taken from the last 19 values.
 *   The recurrence is applied using only positive intermediate sums,
 *   avoiding unsigned overflows from negative coefficients.
 */
#include <stddef.h>
#include <stdbool.h>

/* number of possible row patterns (3 columns with values 0..2) */
#define NUM_ROWS 27
/* number of state pairs (row_a,row_b) */
#define NUM_STATES (NUM_ROWS * NUM_ROWS)
/* maximum offset in recurrence (a(n-3) … a(n-19)) */
#define REC_MAX_OFF 19
#define REC_MIN_OFF 3
#define REC_ORDER (REC_MAX_OFF - REC_MIN_OFF + 1)  /* 17 */

/* tables for row digit extraction, S values and horizontal condition */
static unsigned char row_digits[NUM_ROWS][3];
static unsigned char S0[NUM_STATES];
static unsigned char S1[NUM_STATES];
static bool horiz_ok[NUM_STATES];
static bool tables_inited = false;

static void init_tables(void)
{
    if (tables_inited) return;
    /* encode each row pattern in base‑3 */
    for (int v = 0; v < NUM_ROWS; ++v) {
        int x = v;
        for (int j = 0; j < 3; ++j) {
            row_digits[v][j] = x % 3;
            x /= 3;
        }
    }
    /* compute S values for every pair of consecutive rows and
       whether horizontal adjacency holds (|S0‑S1|==1) */
    for (int a = 0; a < NUM_ROWS; ++a) {
        for (int b = 0; b < NUM_ROWS; ++b) {
            int idx = a * NUM_ROWS + b;
            unsigned char s2[2];
            for (int j = 0; j < 2; ++j) {
                int vals[4];
                vals[0] = row_digits[a][j];
                vals[1] = row_digits[a][j + 1];
                vals[2] = row_digits[b][j];
                vals[3] = row_digits[b][j + 1];
                /* compute max */
                unsigned char m = vals[0];
                for (int k = 1; k < 4; ++k) if (vals[k] > m) m = vals[k];
                /* find lower median (second smallest) by sorting 4 values */
                int t;
                for (int p = 0; p < 3; ++p)
                    for (int q = p + 1; q < 4; ++q)
                        if (vals[p] > vals[q]) {
                            t = vals[p]; vals[p] = vals[q]; vals[q] = t;
                        }
                unsigned char lower = vals[1];
                s2[j] = m + lower;
            }
            S0[idx] = s2[0];
            S1[idx] = s2[1];
            int diff = s2[0] > s2[1] ? s2[0] - s2[1] : s2[1] - s2[0];
            horiz_ok[idx] = (diff == 1);
        }
    }
    tables_inited = true;
}

/* base values a(1)…a(19) computed once via DP */
static seq_t base_vals[19];
static bool base_inited = false;

static void init_base(void)
{
    if (base_inited) return;
    init_tables();
    static seq_t dp_cur[NUM_STATES];
    static seq_t dp_next[NUM_STATES];
    for (int i = 0; i < NUM_STATES; ++i) dp_cur[i] = 0;
    /* initial pairs produce the a(1) values (subblock 0) */
    for (int a = 0; a < NUM_ROWS; ++a)
        for (int b = 0; b < NUM_ROWS; ++b) {
            int idx = a * NUM_ROWS + b;
            if (horiz_ok[idx]) dp_cur[idx] = (seq_t)1;
        }

    /* store a(1) */
    seq_t sum = 0;
    for (int i = 0; i < NUM_STATES; ++i) sum += dp_cur[i];
    base_vals[0] = sum;

    /* iterate to generate a(2)…a(19) (18 transitions) */
    for (int step = 1; step < 19; ++step) {
        for (int i = 0; i < NUM_STATES; ++i) dp_next[i] = 0;
        for (int a = 0; a < NUM_ROWS; ++a)
            for (int b = 0; b < NUM_ROWS; ++b) {
                int idx_cur = a * NUM_ROWS + b;
                seq_t cnt = dp_cur[idx_cur];
                if (!cnt) continue;
                unsigned char Sp0 = S0[idx_cur];
                unsigned char Sp1 = S1[idx_cur];
                for (int c = 0; c < NUM_ROWS; ++c) {
                    int idx_next = b * NUM_ROWS + c;
                    unsigned char Sc0 = S0[idx_next];
                    unsigned char Sc1 = S1[idx_next];
                    int d0 = Sp0 > Sc0 ? Sp0 - Sc0 : Sc0 - Sp0;
                    if (d0 != 1) continue;
                    int d1 = Sp1 > Sc1 ? Sp1 - Sc1 : Sc1 - Sp1;
                    if (d1 != 1) continue;
                    int h = Sc0 > Sc1 ? Sc0 - Sc1 : Sc1 - Sc0;
                    if (h != 1) continue;
                    dp_next[idx_next] += cnt;
                }
            }
        /* copy next to current */
        for (int i = 0; i < NUM_STATES; ++i) dp_cur[i] = dp_next[i];
        /* compute a(n) for this step */
        sum = 0;
        for (int i = 0; i < NUM_STATES; ++i) sum += dp_cur[i];
        base_vals[step] = sum;   /* step 1 → a(2), step 18 → a(19) */
    }
    base_inited = true;
}

/* coefficients for the recurrence a(n) = Σ coeff * a(n-k)  (k = 3..19) */
static const int coeffs[REC_ORDER] = {
    34,   172,  254,   282,  -973, -4066, -3238,  3634,
    13908,5602, -22988,-3264, 22488, -7392, -13696, 4608, 4096
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* sequence is undefined for 0 */
    init_base();

    if (n <= 19) return base_vals[n - 1];

    /* circular buffer of last 19 values a(current-18)…a(current) */
    seq_t buf[19];
    for (int i = 0; i < 19; ++i) buf[i] = base_vals[i];

    for (idx_t i = 20; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int off = REC_MIN_OFF; off <= REC_MAX_OFF; ++off) {
            int idx_needed = REC_MAX_OFF - off;          /* 19‑off : 0…16 */
            int coef = coeffs[off - REC_MIN_OFF];
            if (coef > 0)
                pos += (seq_t)coef * buf[idx_needed];
            else
                neg += (seq_t)(-coef) * buf[idx_needed];
        }
        seq_t tot = pos - neg;     /* assumes pos ≥ neg (recurrence holds) */

        /* shift buffer */
        for (int j = 0; j < 18; ++j) buf[j] = buf[j + 1];
        buf[18] = tot;
    }
    return buf[18];          /* a(n) */
}
