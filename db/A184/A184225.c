/*
 * A184225  Half the number of (n+1)X4 0..3 arrays with every 2X2 subblock
 *          diagonal sum differing from its antidiagonal sum by more than 3.
 * https://oeis.org/A184225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Half the number of (n+1)×4 0..3 arrays with every 2×2 subblock
 * diagonal sum differing from its antidiagonal sum by more than 3.
 *
 * Recurrence (empirical):
 *   a(n) = 10*a(n-1)+24*a(n-2)-241*a(n-3)+55*a(n-4)+1084*a(n-5)
 *         -594*a(n-6)-1710*a(n-7)+1050*a(n-8)+928*a(n-9)-552*a(n-10)
 *         -132*a(n-11)+72*a(n-12)
 * with initial values:
 *   a(1)=180, a(2)=2318, a(3)=20958, a(4)=224156, a(5)=2227254,
 *   a(6)=22904858, a(7)=231988508, a(8)=2366228756, a(9)=24059229300,
 *   a(10)=244980260166, a(11)=2492868966664, a(12)=25374399195344
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Pre‑computed compatibility table and digit extraction             */
/* ------------------------------------------------------------------ */
static bool compat_initialized = false;
static uint8_t compat_matrix[256][256];
static uint8_t digits[256][4];

static void init_compat_matrix(void)
{
    if (compat_initialized)
        return;

    /* precompute digits of every 4‑tuple (0..3 values, 2 bits each) */
    for (int x = 0; x < 256; ++x) {
        for (int k = 0; k < 4; ++k)
            digits[x][k] = (x >> (k * 2)) & 3U;
    }

    /* build compatibility matrix */
    for (int i = 0; i < 256; ++i) {
        for (int j = 0; j < 256; ++j) {
            uint8_t ok = 1;
            for (int col = 0; col < 3; ++col) {
                uint8_t diag = digits[i][col] + digits[j][col + 1];
                uint8_t anti = digits[i][col + 1] + digits[j][col];
                uint8_t diff = diag > anti ? diag - anti : anti - diag;
                if (diff <= 3) {
                    ok = 0;
                    break;
                }
            }
            compat_matrix[i][j] = ok;
        }
    }

    compat_initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Compute the first 12 terms by dynamic programming                */
/* ------------------------------------------------------------------ */
static void init_first_terms(seq_t *terms)
{
    init_compat_matrix();

    /* dp_cur and dp_next hold the number of ways to end with each state */
    seq_t dp_cur[256];
    seq_t dp_next[256];

    for (int i = 0; i < 256; ++i) {
        dp_cur[i] = (seq_t)1;    /* after the first row */
        dp_next[i] = (seq_t)0;
    }

    /* rows = 2 .. 13  corresponds to n = 1 .. 12 */
    for (int rows = 2; rows <= 13; ++rows) {
        for (int i = 0; i < 256; ++i)
            dp_next[i] = 0;

        for (int i = 0; i < 256; ++i) {
            seq_t ways = dp_cur[i];
            if (!ways)
                continue;
            for (int j = 0; j < 256; ++j) {
                if (compat_matrix[i][j])
                    dp_next[j] += ways;
            }
        }

        /* sum all ways for this number of rows and halve */
        seq_t sum = 0;
        for (int j = 0; j < 256; ++j)
            sum += dp_next[j];
        terms[rows - 1] = sum / (seq_t)2;

        /* swap current and next */
        for (int i = 0; i < 256; ++i)
            dp_cur[i] = dp_next[i];
    }
}

/* ------------------------------------------------------------------ */
/*  Public routine                                                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* initial terms are computed once */
    static bool ready = false;
    static seq_t init[13];              /* indices 1..12 used */

    if (!ready) {
        init_first_terms(init);
        ready = true;
    }

    if (n <= 12)
        return init[n];

    /* recurrence coefficients */
    static const int coeff[12] = {
        10, 24, -241, 55, 1084, -594,
        -1710, 1050, 928, -552, -132, 72
    };

    /* buffer holding the last 12 computed values:
     * buf[0] = a(k-1), buf[1] = a(k-2), ..., buf[11] = a(k-12)   */
    seq_t buf[12];
    buf[0] = init[12];
    buf[1] = init[11];
    buf[2] = init[10];
    buf[3] = init[9];
    buf[4] = init[8];
    buf[5] = init[7];
    buf[6] = init[6];
    buf[7] = init[5];
    buf[8] = init[4];
    buf[9] = init[3];
    buf[10] = init[2];
    buf[11] = init[1];

    for (idx_t k = 13; k <= n; ++k) {
        seq_t pos = 0;
        seq_t neg = 0;

        for (int c = 0; c < 12; ++c) {
            int cval = coeff[c];
            seq_t prod;
            if (cval >= 0) {
                prod = buf[c] * (seq_t)(cval);
                pos += prod;
            } else {
                prod = buf[c] * (seq_t)(-cval);
                neg += prod;
            }
        }

        seq_t val = pos - neg;

        /* shift buffer right and insert new value at front */
        for (int i = 11; i > 0; --i)
            buf[i] = buf[i - 1];
        buf[0] = val;
    }

    return buf[0];
}
