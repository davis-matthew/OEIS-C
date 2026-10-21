/*
 * A223618  Number of nX6 0..1 arrays with rows and columns unimodal.
 * https://oeis.org/A223618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of n×6 0..1 arrays with rows and columns unimodal
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 90, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   the DP computes the number of ways to choose n rows, each of which
 *   is a unimodal 6‑bit pattern, while keeping the columns unimodal. */

#include <stdint.h>
#include <stdbool.h>

/* number of bits per column and states per column to represent
 * leading zeros, 1‑block, trailing zeros – encoded in base‑3   */
#define BITS             6
#define STATES           729               /* 3^6 */
#define PATTERNS         22

static const int pow3[BITS] = {1, 3, 9, 27, 81, 243};

static signed char rows[PATTERNS];          /* unimodal row patterns */
static signed short transition[STATES][PATTERNS];
static bool tables_inited = false;

/* test whether a 6‑bit pattern is unimodal */
static bool is_unimodal(uint8_t pat) {
    bool saw1 = false, saw00 = false;
    for (int i = 0; i < BITS; ++i) {
        bool bit = (pat >> i) & 1;
        if (bit) {
            if (saw00) return false;
            saw1 = true;
        } else {
            if (saw1) saw00 = true;
        }
    }
    return true;
}

/* initialise the pattern list and transition table once */
static void init_tables(void) {
    if (tables_inited) return;
    tables_inited = true;

    /* collect the 22 unimodal patterns (including all‑zero) */
    int cnt = 0;
    for (uint8_t pat = 0; pat < 64; ++pat) {
        if (is_unimodal(pat)) rows[cnt++] = pat;
    }

    /* pre‑decode state digits for speed */
    static unsigned char state_digits[STATES][BITS];
    for (int s = 0; s < STATES; ++s) {
        int x = s;
        for (int j = 0; j < BITS; ++j) {
            state_digits[s][j] = x % 3;
            x /= 3;
        }
    }

    /* build the transition table */
    for (int s = 0; s < STATES; ++s) {
        for (int p = 0; p < PATTERNS; ++p) {
            int news = 0;
            bool ok = true;
            for (int j = 0; j < BITS; ++j) {
                unsigned char d = state_digits[s][j];
                bool bit   = (rows[p] >> j) & 1;
                unsigned char nd;
                if (d == 0) {
                    nd = bit ? 1 : 0;
                } else if (d == 1) {
                    nd = bit ? 1 : 2;
                } else {          /* d == 2 */
                    if (bit) { ok = false; break; }
                    nd = 2;
                }
                news += nd * pow3[j];
            }
            transition[s][p] = ok ? (signed short)news : -1;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined for n=0 */

    init_tables();

    /* DP arrays – reuse statically to avoid allocation overhead */
    static seq_t dp[STATES];
    static seq_t newdp[STATES];
    static bool first_call = true;
    static seq_t *cur = dp, *nxt = newdp;

    if (first_call) {
        first_call = false;
        for (int i = 0; i < STATES; ++i) dp[i] = 0;
        cur[0] = (seq_t)1;             /* start with empty matrix */
    } else {
        /* reset the current array for the new call */
        for (int i = 0; i < STATES; ++i) cur[i] = 0;
        cur[0] = (seq_t)1;
    }

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < STATES; ++i) nxt[i] = 0;
        for (int s = 0; s < STATES; ++s) {
            if (!cur[s]) continue;
            for (int p = 0; p < PATTERNS; ++p) {
                signed short t = transition[s][p];
                if (t >= 0) nxt[t] += cur[s];
            }
        }
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    seq_t res = 0;
    for (int s = 0; s < STATES; ++s) res += cur[s];
    return res;
}
