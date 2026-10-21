/*
 * A230922  Number of nX4 0..2 arrays x(i,j) with each element horizontally,
 *          vertically, diagonally or antidiagonally next to at least one
 *          element with value (x(i,j)+1) mod 3 and at least one element with
 *          value (x(i,j)-1) mod 3, and upper left element zero.
 * https://oeis.org/A230922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 39*a(n-1) +624*a(n-2) +4268*a(n-3)
 *            -17754*a(n-4) +128856*a(n-5) -345592*a(n-6) +557424*a(n-7)
 *            -939312*a(n-8) -946592*a(n-9) -489504*a(n-10) -318144*a(n-11)
 *            -475904*a(n-12) -49152*a(n-13) +15360*a(n-14) +4096*a(n-15)
 *            (valid for n>=16) Together with initial values a(1)=0...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Shared type definitions are supplied by the build */
extern seq_t a(idx_t n);

/* --------------------------------------------------------------------- */
/*  Precomputation tables for rows of width 4  (values 0..2)              */
/* --------------------------------------------------------------------- */
#define WIDTH 4
#define ROWS 81   /* 3^4 */

static unsigned char row_vals[ROWS][WIDTH];
static unsigned char top_valid[ROWS][ROWS];
static unsigned char bottom_valid[ROWS][ROWS];
static unsigned char valid[ROWS][ROWS][ROWS];

/* --------------------------------------------------------------------- */
/*  Basic table helpers                                               */
/* --------------------------------------------------------------------- */
static void init_row_vals(void)
{
    for (int code = 0; code < ROWS; ++code) {
        int tmp = code;
        for (int c = 0; c < WIDTH; ++c) {
            row_vals[code][c] = tmp % 3;
            tmp /= 3;
        }
    }
}

static bool cell_ok(int up, int cur, int down, int col, int plus, int minus)
{
    /* left neighbor */
    if (col > 0) {
        unsigned char n = row_vals[cur][col-1];
        if (n == plus) return true;
        if (n == minus) return true;
    }
    /* right neighbor */
    if (col < WIDTH-1) {
        unsigned char n = row_vals[cur][col+1];
        if (n == plus) return true;
        if (n == minus) return true;
    }
    /* up-left */
    if (up != -1 && col > 0) {
        unsigned char n = row_vals[up][col-1];
        if (n == plus) return true;
        if (n == minus) return true;
    }
    /* up */
    if (up != -1) {
        unsigned char n = row_vals[up][col];
        if (n == plus) return true;
        if (n == minus) return true;
    }
    /* up-right */
    if (up != -1 && col < WIDTH-1) {
        unsigned char n = row_vals[up][col+1];
        if (n == plus) return true;
        if (n == minus) return true;
    }
    /* down-left */
    if (down != -1 && col > 0) {
        unsigned char n = row_vals[down][col-1];
        if (n == plus) return true;
        if (n == minus) return true;
    }
    /* down */
    if (down != -1) {
        unsigned char n = row_vals[down][col];
        if (n == plus) return true;
        if (n == minus) return true;
    }
    /* down-right */
    if (down != -1 && col < WIDTH-1) {
        unsigned char n = row_vals[down][col+1];
        if (n == plus) return true;
        if (n == minus) return true;
    }
    return false;
}

static bool row_valid(int up, int cur, int down)
{
    for (int col = 0; col < WIDTH; ++col) {
        unsigned char val = row_vals[cur][col];
        int plus  = (val + 1) % 3;
        int minus = (val + 2) % 3;
        bool has_plus = false, has_minus = false;
        /* left/right / up/down / diagonals */
        /* left */
        if (col > 0) {
            unsigned char n = row_vals[cur][col-1];
            if (n == plus) has_plus = true;
            else if (n == minus) has_minus = true;
        }
        /* right */
        if (col < WIDTH-1) {
            unsigned char n = row_vals[cur][col+1];
            if (n == plus) has_plus = true;
            else if (n == minus) has_minus = true;
        }
        /* up-left */
        if (up != -1 && col > 0) {
            unsigned char n = row_vals[up][col-1];
            if (n == plus) has_plus = true;
            else if (n == minus) has_minus = true;
        }
        /* up */
        if (up != -1) {
            unsigned char n = row_vals[up][col];
            if (n == plus) has_plus = true;
            else if (n == minus) has_minus = true;
        }
        /* up-right */
        if (up != -1 && col < WIDTH-1) {
            unsigned char n = row_vals[up][col+1];
            if (n == plus) has_plus = true;
            else if (n == minus) has_minus = true;
        }
        /* down-left */
        if (down != -1 && col > 0) {
            unsigned char n = row_vals[down][col-1];
            if (n == plus) has_plus = true;
            else if (n == minus) has_minus = true;
        }
        /* down */
        if (down != -1) {
            unsigned char n = row_vals[down][col];
            if (n == plus) has_plus = true;
            else if (n == minus) has_minus = true;
        }
        /* down-right */
        if (down != -1 && col < WIDTH-1) {
            unsigned char n = row_vals[down][col+1];
            if (n == plus) has_plus = true;
            else if (n == minus) has_minus = true;
        }
        if (!(has_plus && has_minus))
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  Build auxiliary tables                                           */
/* --------------------------------------------------------------------- */
static void build_tables(void)
{
    init_row_vals();

    for (int up = 0; up < ROWS; ++up) {
        for (int cur = 0; cur < ROWS; ++cur) {
            for (int down = 0; down < ROWS; ++down) {
                valid[up][cur][down] = row_valid(up, cur, down);
            }
        }
    }
    for (int cur = 0; cur < ROWS; ++cur) {
        for (int nxt = 0; nxt < ROWS; ++nxt) {
            top_valid[cur][nxt] = row_valid(-1, cur, nxt);
        }
    }
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int cur = 0; cur < ROWS; ++cur) {
            bottom_valid[prev][cur] = row_valid(prev, cur, -1);
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Compute the first 15 terms using transfer‑matrix DP                 */
/* --------------------------------------------------------------------- */
static seq_t init_terms[15];

static void compute_first_terms(void)
{
    /* Initialize tables */
    build_tables();

    /* a(1) is 0 */
    init_terms[0] = 0;

    /* DP tables: dp[prev][cur] */
    seq_t dp[ROWS][ROWS] = {0};
    seq_t dp_next[ROWS][ROWS];

    /* Step n = 2: initial rows r1 (top-left 0) and r2 */
    for (int r1 = 0; r1 < ROWS; ++r1) {
        if (row_vals[r1][0] != 0) continue;   /* upper-left fixed to 0 */
        for (int r2 = 0; r2 < ROWS; ++r2) {
            if (top_valid[r1][r2]) {
                dp[r1][r2] = dp[r1][r2] + 1;
            }
        }
    }

    /* Compute a(2) */
    seq_t total = 0;
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int cur = 0; cur < ROWS; ++cur) {
            if (dp[prev][cur] == 0) continue;
            if (bottom_valid[prev][cur])
                total = total + dp[prev][cur];
        }
    }
    init_terms[1] = total;  /* a(2) */

    /* Steps n = 3 .. 15 */
    for (int n = 3; n <= 15; ++n) {
        /* reset dp_next */
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                dp_next[i][j] = 0;

        /* transition */
        for (int prev = 0; prev < ROWS; ++prev) {
            for (int cur = 0; cur < ROWS; ++cur) {
                seq_t cnt = dp[prev][cur];
                if (cnt == 0) continue;
                for (int nxt = 0; nxt < ROWS; ++nxt) {
                    if (valid[prev][cur][nxt]) {
                        dp_next[cur][nxt] = dp_next[cur][nxt] + cnt;
                    }
                }
            }
        }

        /* move to next */
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                dp[i][j] = dp_next[i][j];

        /* compute a(n) */
        total = 0;
        for (int prev = 0; prev < ROWS; ++prev) {
            for (int cur = 0; cur < ROWS; ++cur) {
                seq_t cnt = dp[prev][cur];
                if (cnt == 0) continue;
                if (bottom_valid[prev][cur])
                    total = total + cnt;
            }
        }
        init_terms[n-1] = total;
    }
}

/* --------------------------------------------------------------------- */
/*  Recurrence coefficients                                           */
/* --------------------------------------------------------------------- */
static const int coeffs[15] = {
    39, 624, 4268, -17754, 128856,
    -345592, 557424, -939312, -946592,
    -489504, -318144, -475904, -49152,
    15360, 4096
};

/* --------------------------------------------------------------------- */
/*  Main sequence function                                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool ready = false;
    if (!ready) {
        compute_first_terms();
        ready = true;
    }

    if (n == 0) return (seq_t)0;    /* should not happen (offset 1) */
    if (n <= 15) return init_terms[n-1];

    /* Prepare sliding window of the last 15 terms */
    seq_t window[15];
    for (int i = 0; i < 15; ++i)
        window[i] = init_terms[14-i];   /* window[0]=a(15), window[14]=a(1) */

    /* Iteratively compute a(n) */
    for (idx_t cur = 16; cur <= n; ++cur) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 0; k < 15; ++k) {
            seq_t term = window[k];
            int c = coeffs[k];
            if (c >= 0)
                pos = pos + term * (seq_t)c;
            else
                neg = neg + term * (seq_t)(-c);
        }
        seq_t next = pos - neg;

        /* shift window */
        for (int k = 14; k > 0; --k)
            window[k] = window[k-1];
        window[0] = next;
    }

    return window[0];
}
